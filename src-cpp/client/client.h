#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <cstdlib>
#include <system_error>
#include <asio.hpp>

#define LEN 1024
using asio::ip::udp;

class Client {
  public:
    Client(asio::io_service& io_service, int p);
    void start();

  private:
    int port;
    udp::socket socket;
    udp::endpoint sender_endpoint;
    char data[LEN];
    void receive();



};

#endif
