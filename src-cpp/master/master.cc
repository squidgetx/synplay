#include "master.h"
#include "asio.hpp"

#include <string>

using namespace std;
using namespace asio::ip;

void Master::send_packet(size_t length, char *data){

  socket.async_send_to(
      asio::buffer(data, length), endpt,
      [this](error_code /*ec*/, size_t /*bytes_sent*/)
      {
        // send more data
      });
}

void Master::send(){
  char *data = "this is a test";
  this->send_packet(strlen(data),data);
}
