#include "master.h"
#include "asio.hpp"

#include <string>

using namespace std;
using namespace asio::ip;

Master::Master(string ip_addr, int port){
  endpt = udp::endpoint(address::from_string(ip_addr),port);
}

void connect(asio::io_service io_s){
  io_service = io_s;
  socket = udp::socket(io_service,enpt);
}

void Master::send_packet(size_t length, char *data){
  asio::buffer buf = asio::buffer(data,length);

  socket.async_send_to(
      buf, endpt,
      [this](error_code /*ec*/, size_t /*bytes_sent*/)
      {
        // send more data
      });
}

void Master::send(){
  char *data = "this is a test";
  this->send_packet(strlen(data),data);
}
