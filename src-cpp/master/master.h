#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <iostream>
#include <fstream>
#include "asio.hpp"
#include "util/mpacket.h"
#include "net/time_packet.h"
#include <sndfile.hh>

#define BUFFER_SIZE (1024)
#define TP_BUFFER_SIZE (5 * 8)

class Master
{
  public:
    Master(std::string &ip_addr, uint16_t port, std::string filename) :
      io_service(),
      remote_endpt(asio::ip::address::from_string(ip_addr),port),
      socket(io_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)){

          file = SndfileHandle (filename);

          std::cerr << "Opened file '" << filename << "'" << std::endl;
          std::cerr << "\tSample rate '" << file.samplerate() << "'" << std::endl;
          std::cerr << "\tChannels '" << file.channels() << "'" << std::endl;
      }
    
    void run();

    ~Master ();

  private:
    void send();
    void send_packet(const MPacket& mp);
    void receive_timesync_reply();
    void send_timesync();

    asio::io_service io_service;
    asio::ip::udp::endpoint remote_endpt;
    asio::ip::udp::socket socket;
    SndfileHandle file;
    uint8_t tp_buffer[TP_BUFFER_SIZE];
};

#endif
