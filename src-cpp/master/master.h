#include <string>
#include "asio.hpp"

#ifndef MASTER_H
#define MASTER_H

class Master
{
  public:
    Master(std::string &ip_addr, int port) :
      io_service(),
      remote_endpt(asio::ip::address::from_string(ip_addr),port),
      socket(io_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)){

      }
    
    void run();

    ~Master ();

  private:
    void send();
    void send_packet(std::size_t length, char *data);

    asio::io_service io_service;
    asio::ip::udp::endpoint remote_endpt;
    asio::ip::udp::socket socket;
};

#endif
