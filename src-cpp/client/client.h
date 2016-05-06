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
    Client(asio::io_service& io_service, int p);
    void start();

  private:
    void receive();
    void receiveFromFile();

    int port;
    udp::socket socket;
    udp::endpoint sender_endpoint;
    char data[LEN];
    RingBuffer<MPacket*> packet_buffer;
    std::deque<short> play_buffer;
    short file_buf[BUFFER_LENGTH];
    SndfileHandle file;
};

#endif
