#include "asio.hpp"

#include "master/master.h"
#include "util/syntime.h"
#include "util/mpacket.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

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
