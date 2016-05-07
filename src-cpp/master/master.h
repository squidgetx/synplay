#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <iostream>
#include <fstream>
#include "asio.hpp"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>
#include "master/msocket.h"

#define BUFFER_SIZE (1024)
#define TP_BUFFER_SIZE (5 * 8)

class Master
{
  public:
    Master(std::string& filename,std::vector<asio::ip::udp::endpoint>& remote_endpts);
    ~Master ();

    void run();

  private:
    asio::io_service io_service;
    std::vector<asio::ip::udp::endpoint> remote_endpts;
    std::vector<MSocket *> msockets;
};

#endif
