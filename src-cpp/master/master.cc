#include "asio.hpp"

#include "master/master.h"
#include "util/syntime.h"
#include "net/mpacket.h"
#include "net/time_packet.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

void Master::send_timesync() {
  TPacket tp;
  tp.from_sent = get_millisecond_time();

  // send the packet to the client
  socket.async_send_to(
      asio::buffer(tp.pack()), remote_endpt,
      [this](error_code, size_t) {
        this->receive_timesync_reply();
      }
  );
}

void Master::receive_timesync_reply() {
  socket.async_receive_from(
      asio::buffer(tp_buffer, TP_BUFFER_SIZE), remote_endpt,
      [this](error_code e, size_t bytes_recvd) {
        // calculate sum shit

        // immediately grab the receipt time  
        mtime_t from_recv = get_millisecond_time();
        
        // unpack the time packet
        Packet * p = Packet::unpack(tp_buffer, bytes_recvd);
        TPacket * tp = static_cast<TPacket *> (p);
        tp->from_recvd = from_recv;

        // calculate the offset
        mtime_t offset = ((tp->to_recvd - tp->from_sent) - (tp->to_sent - tp->from_recvd))/2;
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
        send_data();
      }
  );
}

static int sent = 0;

void Master::send_data(){
  sent++;
  int16_t *buf = new int16_t[BUFFER_SIZE] ;

  sf_count_t num_read = file.read (buf, BUFFER_SIZE) ;

  if (!num_read){
    return;
  }

  time_t now = get_millisecond_time();
  MPacket mp(now,buf,num_read);
 // std::cerr << "sending ";
 // mp.print_all();
  socket.async_send_to(
    asio::buffer(mp.pack()), remote_endpt,
    [this](error_code /*ec*/, size_t /*bytes_sent*/)
    {
      this->send_data();
    });

//  std::cout << sent << std::endl;
}

void Master::run(){
  this->send_timesync();
  io_service.run();
}
