#ifndef _MPACKET_H_
#define _MPACKET_H_

#define PACKET_SIZE 1024

#include <iostream>
#include <sstream>
#include "asio.hpp"
#include "bytebuffer.h"

class MPacket {
  public:
    MPacket(time_t time, int16_t* p, size_t psize);
    int16_t* get_payload();
    size_t get_payload_size();
    time_t get_timestamp();
    void print();
    void print_all();
    asio::const_buffer pack() const;
    static MPacket* unpack(uint8_t * p, std::size_t n);

  private:
    int16_t* payload;
    size_t payload_size;
    time_t timestamp;
};


#endif
