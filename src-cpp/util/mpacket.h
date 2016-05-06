#ifndef _MPACKET_H_
#define _MPACKET_H_

#define PACKET_SIZE 1024

#include <iostream>
#include <sstream>
#include "asio.hpp"
#include "bytebuffer.h"

class MPacket {
  public:
    MPacket(long time, short* p, int psize);
    short* get_payload();
    int get_payload_size();
    long get_timestamp();
    void print();
    asio::const_buffer pack() const;
    static MPacket* unpack(char * p, std::size_t n);

  private:
    short* payload;
    int payload_size;
    long timestamp;

};


#endif
