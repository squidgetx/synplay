#include "master.h"
#include "asio.hpp"

#include <string>

using namespace std;
using namespace asio::ip;

void Master::send_packet(size_t length, char *data){

  socket.async_send_to(
      asio::buffer(data, length), remote_endpt,
      [this](error_code /*ec*/, size_t /*bytes_sent*/)
      {
        this->send();
      });
}

void Master::send(){
  char *buf = new char[BUFFER_SIZE];
  file.read(buf,BUFFER_SIZE);

  this->send_packet(BUFFER_SIZE,buf);
}

void Master::run(){
  io_service.run();

  this->send();
}
