#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "asio.hpp"
#include "net/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>

#define BUFFER_SIZE (1024)
#define TP_BUFFER_SIZE (2 + 5 * 8)

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

    std::vector<uint64_t> packet_count;

    uint8_t tp_buffer[TP_BUFFER_SIZE];
    int16_t data_buffer[BUFFER_SIZE];
    uint16_t synced;
    uint16_t outstanding_packets;

    void send_data(asio::ip::udp::endpoint& remote_endpt, asio::const_buffer& buf, /* debugging */ uint64_t &sent);
    void send_data();

    void send_timesync(asio::ip::udp::endpoint& remote_endpt);
    void send_timesync();
    void receive_timesync_reply(asio::ip::udp::endpoint& remote_endpt);
};

#endif
