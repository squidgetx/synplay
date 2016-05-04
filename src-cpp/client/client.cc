#include "client.h"

void Client::receive() {
  socket.async_receive_from(
    asio::buffer(data, 1024), sender_endpoint,
    [this](std::error_code ec, std::size_t bytes_recvd)
    {
      if (!ec && bytes_recvd > 0)
      {
        for(int i = 0; i < bytes_recvd; i++) {
          std::cout << data[i] << std::endl;
        }
      }
      receive();
    });
}

Client::Client(asio::io_service& io_service, int p) : port(p), 
  socket(io_service, udp::endpoint(udp::v4(), p)) {
}

void Client::start() {
  std::cout << "Listening on " << port << std::endl;

  receive();

  // listen for a packet
  // print its contents!

}


