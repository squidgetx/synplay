#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <cstdlib>
#include <system_error>
#include <deque>
#include <asio.hpp>
#include <portaudio.h>
#include <sndfile.hh>
#include "util/ringbuf.h"
#include "util/mpacket.h"

#define LEN 1024
#define BUFFER_LENGTH 1024
#define SAMPLE_RATE 44100
using asio::ip::udp;

class Client {
  public:
    Client(asio::io_service& io_s, uint16_t p);
    void start();

  private:
    void receive();
    void receiveFromFile();

    uint16_t port;
    udp::socket socket;
    udp::endpoint sender_endpoint;
    uint8_t data[LEN];
    RingBuffer<MPacket*> packet_buffer;
    std::deque<int16_t> play_buffer;
    int16_t file_buf[BUFFER_LENGTH];
    SndfileHandle file;
    asio::io_service& io_service;
};

#endif
