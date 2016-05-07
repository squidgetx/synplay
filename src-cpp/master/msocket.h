#ifndef MSOCKET_H
#define MSOCKET_H

#include <string>
#include <iostream>
#include <fstream>
#include "asio.hpp"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>

#define BUFFER_SIZE (1024)
#define TP_BUFFER_SIZE (5 * 8)

class MSocket
{
  public:
    MSocket(asio::io_service& io_s, asio::ip::udp::endpoint& r_endpt, std::string& fname);
    void start();

    ~MSocket ();

  private:
    void send_packet(const MPacket& mp);
    void send_data();

    void send_timesync();
    void receive_timesync_reply();

    asio::ip::udp::endpoint remote_endpt;
    asio::ip::udp::socket socket;
    SndfileHandle file;
    uint8_t tp_buffer[TP_BUFFER_SIZE];
};

#endif
