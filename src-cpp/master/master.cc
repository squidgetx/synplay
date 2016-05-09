#include "asio.hpp"
#include <sndfile.hh>

#include <chrono>
#include <string>
#include <thread>
#include <memory>

#include "master/master.h"
#include "net/packet.h"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include "util/syntime.h"
#include "util/assert.h"

using namespace std;
using namespace asio::ip;

Master::Master(string& fname, vector<udp::endpoint>& r_endpts) :
  io_service(), socket(io_service,udp::endpoint(udp::v4(),0)), synced(0), outstanding_packets(0)
{
  for (auto &endpoint : r_endpts) {
    connections.insert({std::make_shared<udp::endpoint>(endpoint),{}});
  }

  file = SndfileHandle (fname);

  std::cerr << "Opened file '" << fname << "'" << std::endl;
  std::cerr << "\tSample rate '" << file.samplerate() << "'" << std::endl;
  std::cerr << "\tChannels '" << file.channels() << "'" << std::endl;
}

Master::~Master(){
}

void Master::receive_everything() {
  std::shared_ptr<udp::endpoint> remote_endpt = std::shared_ptr<udp::endpoint>();
  socket.async_receive_from(
      asio::buffer(this->tp_buffer, TP_BUFFER_SIZE), *remote_endpt,
      [this,remote_endpt](error_code e, size_t bytes_recvd) {
        // immediately grab the receipt time
        mtime_t from_recv = get_millisecond_time();

        // unpack the time packet
        Packet * p = Packet::unpack(tp_buffer, bytes_recvd);
        check_or_die(p->get_type() == PacketType::TIME);
        TPacket * tp = static_cast<TPacket *> (p);

        // Demultiplex
        MConnection &cxn = connections[remote_endpt];

        // Cancel the timer, if any
        if (cxn.timer != nullptr) {
          cxn.timer->cancel();
          delete cxn.timer;
          cxn.timer = nullptr;
        }

        // Reset attempts
        cxn.attempts = 0;

        bool continueReceive = true;

        switch (cxn.state) {
          case MConnection::NAKED:
            // No one should send to us
            break;
          case MConnection::SENT_INITIAL_TIMESYNC:
          {
            // Got the reply to the initial timesync
            tp->from_recvd = from_recv;
            tp->tp_type = COMPLETE;
            mtime_offset_t offset =
              ((static_cast<mtime_offset_t> (tp->to_recvd) -
                static_cast<mtime_offset_t> (tp->from_sent)) +
               (static_cast<mtime_offset_t> (tp->to_sent) -
                static_cast<mtime_offset_t> (tp->from_recvd)))/2;
            tp->offset = offset;

            this->send_final_timesync(remote_endpt, tp, cxn);
            break;
          }
          case MConnection::SENT_FINAL_TIMESYNC:
            // Got the reply to the final timesync
            cxn.state = MConnection::PENDING_ALL_SYNCED;
            synced++;
            if (synced == connections.size()) {
              // All cxns are pending all synced!
              for (auto& kv : connections) {
                // Sanity check: make sure that all the states are indeed
                // PENDING_ALL_SYNCED
                check_or_die(kv.second.state == MConnection::PENDING_ALL_SYNCED);
                kv.second.state = MConnection::SENDING_DATA;
              }
              continueReceive = false;
              send_data();
            }
            break;
          case MConnection::PENDING_ALL_SYNCED:
            // Shouldn't be getting any packets in this state
            break;
          case MConnection::SENDING_DATA:
            // Shouldn't be getting any packets in this state
            break;
          default:
            // Shouldn't be getting any packets in this state
            break;
        }

        if (continueReceive) {
          receive_everything();
        }
      }
  );
}

// send a timesync to every remote endpoint.
void Master::send_timesync() {
  for (auto& kv : connections) {
    send_initial_timesync(kv.first, kv.second);
  }
}

template <typename WaitHandler>
asio::deadline_timer* Master::start_timer(asio::ip::udp::endpoint& remote_endpt, WaitHandler handler){
  // register timeout
  asio::deadline_timer *timer = new asio::deadline_timer(io_service);
  timer->expires_from_now(boost::posix_time::seconds(1));
  timer->async_wait([this,&remote_endpt,handler](const std::error_code& error){
    if (!error){
      cerr << "timeout: " << remote_endpt << endl;
      handler();
    }
  });
  return timer;
}


void Master::send_initial_timesync(std::shared_ptr<udp::endpoint> remote_endpt, MConnection &cxn) {

  cxn.state = MConnection::SENT_INITIAL_TIMESYNC;

  asio::deadline_timer *timer = this->start_timer(remote_endpt, [this,&remote_endpt,&cxn](){
        this->send_initial_timesync(remote_endpt, cxn);
  });

  if (cxn.timer != NULL) {
    cxn.timer->cancel();
    delete cxn.timer;
  }
  cxn.timer = timer;

  if (cxn.attempts > 2) {
    cerr << "send_initial_timesync give up on: " << remote_endpt << endl;
    return;
  } else {
    cerr << "send_initial_timesync remote_endpt = " << remote_endpt << ", attempt = " << cxn.attempts << endl;
  }

  cxn.attempts++;

  TPacket tp;
  tp.from_sent = get_millisecond_time();

  socket.async_send_to(asio::buffer(tp.pack()), *remote_endpt, NULL);
}

void Master::send_final_timesync(std::shared_ptr<asio::ip::udp::endpoint> remote_endpt, TPacket *tp, MConnection &cxn){

    cxn.state = SENT_FINAL_TIMESYNC;

    // Send the final timesync and start a timer

    asio::deadline_timer *timer = this->start_timer(remote_endpt, [this,&remote_endpt](){
          this->send_final_timesync(remote_endpt, tp, cxn);
    });
    if (cxn.timer != NULL) {
      cxn.timer->cancel();
      delete cxn.timer;
    }
    cxn.timer = timer;

    if (cxn.attempts > 2){
      cerr << "send_final_timesync give up on: " << remote_endpt << endl;
      return;
    } else {
      cerr << "send_final_timesync remote_endpt = " << remote_endpt << ", attempt = " << cxn.attempts << endl;
    }

    cxn.attempts++;

    // and send the reply
    socket.async_send_to(
      asio::buffer(tp->pack()), remote_endpt,
      [](error_code, size_t) {
        // reply sent...
      }
    );
}

void Master::send_data(std::shared_ptr<udp::endpoint> remote_endpt, asio::const_buffer& buf){

  socket.async_send_to(asio::buffer(buf), *remote_endpt,
    [this](error_code ec, size_t /*bytes_sent*/){
      // QUESTION: this sends more to everyone in the callback for
      // one successful sent packet... what to do instead?
        if (ec){
            cerr << ec.message() << endl;
        } else if (--this->outstanding_packets == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_TIME_MS / SEND_SPEED_FACTOR));
            this->send_data();
        }
    });
//  std::cout << sent << std::endl;
}

void Master::send_data(){
  outstanding_packets = connections.size();

  if (stream_start == 0) {
    stream_start = get_millisecond_time() + STREAM_OFFSET;
  }

  sf_count_t num_read = file.read (data_buffer, SAMPLES_PER_PACKET) ;

  if (!num_read){
    isDone = true;
    return;
  }

  stream_start = stream_start + PACKET_TIME_MS;
  n_frames_sent += FRAMES_PER_PACKET;
  MPacket mp(stream_start, data_buffer, SAMPLES_PER_PACKET);
  std::cerr << "sending ";
  mp.print();

  asio::const_buffer mp_buf = mp.pack();

  for (auto& it : connections) {
    send_data(it.first, mp_buf);
  }
}

void Master::run(){
  send_timesync();

  while (!isDone)
    io_service.run();
}
