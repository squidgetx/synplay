#include <string>
#include "asio.hpp"

#ifndef MASTER_H
#define MASTER_H

class Master
{
  public:
    Master(asio::io_service &io_service, std::string &ip_addr, int port) :
      endpt(asio::ip::address::from_string(ip_addr),port),
      socket(io_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)){

      }
    void send();

    ~Master ();

  private:
    void send_packet(std::size_t length, char *data);

    asio::ip::udp::endpoint endpt;
    asio::ip::udp::socket socket;
};

#endif
