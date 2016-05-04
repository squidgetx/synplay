#include <string>
#include <iostream>
#include <fstream>
#include "asio.hpp"

#ifndef MASTER_H
#define MASTER_H

#define BUFFER_SIZE (1024)

class Master
{
  public:
    Master(std::string &ip_addr, int port, std::string filename) :
      io_service(),
      remote_endpt(asio::ip::address::from_string(ip_addr),port),
      socket(io_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)){
       file.open(filename, std::ios::in | std::ios::binary); 
      }
    
    void run();

    ~Master () {
      file.close();
    }

  private:
    void send();
    void send_packet(std::size_t length, char *data);

    asio::io_service io_service;
    asio::ip::udp::endpoint remote_endpt;
    asio::ip::udp::socket socket;
    std::ifstream file;
};

#endif
