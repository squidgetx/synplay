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
    Master(std::string filename,std::vector<asio::ip::udp::endpoint> remote_endpts);

    void run();

    ~Master ();

  private:
    void send_data();
    void send_packet(const MPacket& mp);
    void receive_timesync_reply();
    void send_timesync(asio::ip::udp::socket sock);
    void send_timesyncs();

    asio::io_service io_service;
    std::vector<asio::ip::udp::socket> sockets;
    SndfileHandle file;
    uint8_t tp_buffer[TP_BUFFER_SIZE];
};

#endif
