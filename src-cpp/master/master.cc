#include "asio.hpp"

#include "master/master.h"
#include "util/syntime.h"
#include "util/mpacket.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;
/*
void Master::send_timesync() {
  TPacket tp();
  socket.async_send_to(
      asio::buffer(tp.pack()), remote_endpt,
      [this](error_code, size_t) {
        this->receive_timesync_reply();
      }
  );
}

void Master::receive_timesync_reply() {
  uint64_t buffer[5];
  socket.async_receive_from(
      asio::buffer(buffer, 5), remote_endpt,
      [this](error_code e, size_t bytes_recvd) {
        // calculate sum shit
        mtime_t from_recv = get_millisecond_time();
        TPacket * tp = TPacket::unpack(buffer);
        mtime_t offset = (tp->to_recv - tp->from_send) - (tp->to_send - from_recv);
        tp->offset = offset;
        socket.async_send_to(
            asio::buffer(tp->pack()), remote_endpt,
            [this](error_code, size_t) {
              // reply sent...
              // do we need timeouts on this shit
            }
        );
      }
  );
}*/

void Master::send(){
  int16_t *buf = new int16_t[BUFFER_SIZE] ;

  sf_count_t num_read = file.read (buf, BUFFER_SIZE) ;

  if (!num_read){
    return;
  }

  time_t now = get_millisecond_time();

  MPacket mp(now,buf,num_read);

  socket.async_send_to(
    asio::buffer(mp.pack()), remote_endpt,
    [this](error_code /*ec*/, size_t /*bytes_sent*/)
    {
      this->send();
    });
}

void Master::run(){
  this->send();
  io_service.run();
}
