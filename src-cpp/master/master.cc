#include "asio.hpp"
#include <sndfile.hh>

#include <chrono>
#include <string>
#include <thread>

#include "master/master.h"
#include "net/packet.h"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include "util/syntime.h"
#include "util/assert.h"

using namespace std;
using namespace asio::ip;

Master::Master(string& fname,vector<udp::endpoint>& r_endpts) :
  remote_endpts(r_endpts), io_service(), socket(io_service,udp::endpoint(udp::v4(),0)), synced(0), outstanding_packets(0)
{
  file = SndfileHandle (fname);
  packet_count = {r_endpts.size(), 0};

  std::cerr << "Opened file '" << fname << "'" << std::endl;
  std::cerr << "\tSample rate '" << file.samplerate() << "'" << std::endl;
  std::cerr << "\tChannels '" << file.channels() << "'" << std::endl;
}

Master::~Master(){
}

// send a timesync to every remote endpoint.
void Master::send_timesync(){
  for (udp::endpoint& remote_endpt : remote_endpts) {
    send_initial_timesync(remote_endpt);
  }
}

    template <typename WriteHandler>
asio::deadline_timer *Master::start_timer(asio::ip::udp::endpoint& remote_endpt, int16_t attempt, WriteHandler handler){
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


void Master::send_initial_timesync(udp::endpoint& remote_endpt, int16_t attempt) {

  if (attempt > 2) {
    cerr << "send_initial_timesync give up on: " << remote_endpt << endl;
    return;
  } else {
    cerr << "send_initial_timesync remote_endpt = " << remote_endpt << ", attempt = " << attempt << endl;
  }

  TPacket tp;
  tp.from_sent = get_millisecond_time();

  socket.async_send_to(asio::buffer(tp.pack()),remote_endpt,
      [this,&remote_endpt,attempt](error_code,size_t) {
          this->receive_initial_timesync_reply(remote_endpt,attempt);
      });
}

void Master::receive_initial_timesync_reply(udp::endpoint& remote_endpt, int16_t attempt) {

  asio::deadline_timer *timer = this->start_timer(remote_endpt, attempt, [this,&remote_endpt,attempt](){
      this->send_initial_timesync(remote_endpt, attempt + 1);
    });

  socket.async_receive_from(
      asio::buffer(this->tp_buffer, TP_BUFFER_SIZE), remote_endpt,
      [this,&remote_endpt,timer](error_code e, size_t bytes_recvd){
        // calculate sum shit
        timer->cancel();
        delete timer;

        // immediately grab the receipt time
        mtime_t from_recv = get_millisecond_time();
        // unpack the time packet
        Packet * p = Packet::unpack(tp_buffer, bytes_recvd);
        check_or_die(p->get_type() == PacketType::TIME);
        TPacket * tp = static_cast<TPacket *> (p);
        tp->from_recvd = from_recv;
        tp->tp_type = COMPLETE;

        // calculate the offset
        mtime_offset_t offset = ((static_cast<mtime_offset_t> (tp->to_recvd) - static_cast<mtime_offset_t> (tp->from_sent)) + (static_cast<mtime_offset_t> (tp->to_sent) - static_cast<mtime_offset_t> (tp->from_recvd)))/2;
        tp->offset = offset;

        // attempt to send the final timesync (with the offset calculated).
        this->send_final_timesync(remote_endpt,tp);
      }
  );
}

void Master::send_final_timesync(asio::ip::udp::endpoint& remote_endpt, TPacket *tp, int16_t attempt){


  if (attempt > 2){
    cerr << "send_final_timesync give up on: " << remote_endpt << endl;
    return;
  } else {
    cerr << "send_final_timesync remote_endpt = " << remote_endpt << ", attempt = " << attempt << endl;
  }

  // and send the reply
  socket.async_send_to(
      asio::buffer(tp->pack()), remote_endpt,
      [this,&remote_endpt,tp,attempt](error_code, size_t) {
        // reply sent...
        // do we need timeouts on this shit
        this->receive_final_timesync_reply(remote_endpt,tp,attempt);
      }
    );
}

void Master::receive_final_timesync_reply(asio::ip::udp::endpoint& remote_endpt, TPacket *tp, int16_t attempt){
  asio::deadline_timer *timer = this->start_timer(remote_endpt, attempt, [this,&remote_endpt,tp,attempt](){
      this->send_final_timesync(remote_endpt, tp, attempt + 1);
    });

  socket.async_receive_from(
      asio::buffer(this->tp_buffer, TP_BUFFER_SIZE), remote_endpt,
      [this,&remote_endpt,timer](error_code e, size_t bytes_recvd){
          timer->cancel();
          delete timer;
      });
}


void Master::send_data(udp::endpoint& remote_endpt, asio::const_buffer& buf, /* debug packet counts */ uint64_t &sent){
  socket.async_send_to(asio::buffer(buf),remote_endpt,
    [this,&sent](error_code ec, size_t /*bytes_sent*/){
      // QUESTION: this sends more to everyone in the callback for
      // one successful sent packet... what to do instead?
        sent++;
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
  outstanding_packets = remote_endpts.size();

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

  for (auto&& it = remote_endpts.begin(); it < remote_endpts.end(); it++) {
    send_data(*it, mp_buf, packet_count[std::distance(remote_endpts.begin(), it)]);
  }
}

void Master::run(){
  send_timesync();

  while (!isDone)
    io_service.run();

  std::cerr << "Packet counts" << std::endl;
  for (auto&& it = remote_endpts.begin(); it < remote_endpts.end(); it++) {
    std::cerr << *it << " " <<
      packet_count[std::distance(remote_endpts.begin(), it)] << std::endl;
  }
}
