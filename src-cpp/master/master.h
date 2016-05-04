#include <string>
#include "asio.hpp"

#ifndef MASTER_H
#define MASTER_H

class Master
{
  public:
    Master(std::string ip_addr, int port);

    Master::connect(asio::io_service io_s);
    Master::send();

    ~Master ();

  private:
    void Master::send_packet(std::size_t length, char *data);

    asio::io_service io_service;
    asio::ip::udp::endpoint endpt;
    asio::ip::udp::socket socket;
};

#endif
