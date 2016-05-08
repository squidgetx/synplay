#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <iostream>
#include <fstream>
#include "asio.hpp"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>

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
    asio::ip::udp::socket socket;
    SndfileHandle file;
    uint8_t tp_buffer[TP_BUFFER_SIZE];

    template <typename WriteHandler> 
      void async_send_to(asio::ip::udp::endpoint& remote_endpt, Packet& pack, WriteHandler wh);
    
    void send_data(asio::ip::udp::endpoint& remote_endpt, MPacket &mp);
    void send_data();

    void send_timesync(asio::ip::udp::endpoint& remote_endpt);
    void send_timesync();
    void receive_timesync_reply(asio::ip::udp::endpoint remote_endpt);
};

#endif
