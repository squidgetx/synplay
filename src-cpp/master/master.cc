#include "asio.hpp"

#include "master/master.h"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include "util/syntime.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

Master::Master(string& fname,vector<udp::endpoint>& r_endpts) :
  remote_endpts(r_endpts), io_service(), socket(io_service,udp::endpoint(udp::v4(),0)) 
{
  file = SndfileHandle (fname);

  std::cerr << "Opened file '" << fname << "'" << std::endl;
  std::cerr << "\tSample rate '" << file.samplerate() << "'" << std::endl;
  std::cerr << "\tChannels '" << file.channels() << "'" << std::endl;
}

Master::~Master(){
}

void Master::send_timesync(udp::endpoint& remote_endpt) {
  TPacket tp;
  tp.from_sent = get_millisecond_time();

  socket.async_send_to(asio::buffer(tp.pack()),remote_endpt,
      [this,remote_endpt](error_code,size_t) {
          this->receive_timesync_reply(remote_endpt);
      });
}

// send a timesync to every remote endpoint.
void Master::send_timesync(){
  for (udp::endpoint& remote_endpt : remote_endpts) {
    send_timesync(remote_endpt);
  }
}

void Master::receive_timesync_reply(udp::endpoint remote_endpt) {
  socket.async_receive_from(
      asio::buffer(this->tp_buffer, TP_BUFFER_SIZE), remote_endpt,
      [this,remote_endpt](error_code e, size_t bytes_recvd) {
        // calculate sum shit

        // immediately grab the receipt time  
        mtime_t from_recv = get_millisecond_time();
        
        // unpack the time packet
        Packet * p = Packet::unpack(this->tp_buffer, bytes_recvd);
        TPacket * tp = static_cast<TPacket *> (p);
        tp->from_recvd = from_recv;
        tp->tp_type = COMPLETE;

        // calculate the offset
        mtime_offset_t offset = ((tp->to_recvd - tp->from_sent) + (tp->to_sent - tp->from_recvd))/2;
        tp->offset = offset;

        // and send the reply
        socket.async_send_to(
            asio::buffer(tp->pack()), remote_endpt,
            [this](error_code, size_t) {
              // reply sent...
              // do we need timeouts on this shit
            }
        );
        // ready to start sending data
        this->send_data();
      }
  );
}

void Master::send_data(udp::endpoint& remote_endpt, asio::const_buffer& buf){

  socket.async_send_to(asio::buffer(buf),remote_endpt,
    [this](error_code /*ec*/, size_t /*bytes_sent*/){
      // QUESTION: this sends more to everyone in the callback for
      // one successful sent packet... what to do instead?
      this->send_data();
    });
//  std::cout << sent << std::endl;
}

void Master::send_data(){
  sf_count_t num_read = file.read (data_buffer, BUFFER_SIZE) ;

  if (!num_read){
    return;
  }

  time_t now = get_millisecond_time();
  MPacket mp(now + 1000, data_buffer,num_read);
 // std::cerr << "sending ";
 // mp.print_all();

  asio::const_buffer mp_buf = mp.pack();

  for (udp::endpoint& remote_endpt : remote_endpts) {
    send_data(remote_endpt,mp_buf);
  }
}

void Master::run(){
  send_timesync();
  io_service.run();
}
