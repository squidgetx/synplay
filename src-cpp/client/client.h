#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <cstdlib>
#include <system_error>
#include <asio.hpp>
#include <portaudio.h>
#include "util/ringbuf.h"
#include "util/mpacket.h"

#define LEN 1024
#define SAMPLE_RATE 44100
using asio::ip::udp;

class Client {
  public:
    Client(asio::io_service& io_service, int p);
    void start();

  private:
    int port;
    udp::socket socket;
    udp::endpoint sender_endpoint;
    char data[LEN];
    void receive();
    RingBuffer<MPacket*> packet_buffer;
    RingBuffer<char> play_buffer;



};

#endif
