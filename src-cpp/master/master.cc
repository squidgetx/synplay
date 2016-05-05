#include "asio.hpp"

#include "master/master.h"
#include "util/syntime.h"
#include "util/mpacket.h"

#include <string>

#include <sndfile.hh>

using namespace std;
using namespace asio::ip;

void Master::send_packet(const MPacket& mp){

  socket.async_send_to(
      asio::buffer(mp.pack()), remote_endpt,
      [this](error_code /*ec*/, size_t /*bytes_sent*/)
      {
        this->send();
      });
}

void Master::send(){
	short *buf = new short[BUFFER_SIZE] ;

	long num_read = file.read (buf, BUFFER_SIZE) ;

	if (!num_read){
		return;
	}


  long now = get_millisecond_time();

  MPacket mp(now,buf,num_read);

  this->send_packet(mp);
}

void Master::run(){
  this->send();
  io_service.run();
}
