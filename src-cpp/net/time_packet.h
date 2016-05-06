#ifndef _TIME_PACKET_H_
#define _TIME_PACKET_H_

#include <iostream>
#include <sstream>
#include "asio.hpp"
#include "bytebuffer.h"

typedef mtime_t uint64_t;

typedef struct
{
    mtime_t from_sent;
    mtime_t to_recvd;
    mtime_t to_sent;
    mtime_t from_recvd;
} ntp_time;

class TPacket {
  public:
    TPacket();
    ntp_time get_times();
    asio::const_buffer pack() const;
    static TPacket* unpack(uint64_t* p, std::size_t n);

  private:
    uint64_t* payload;
    size_t payload_size;
    ntp_time times;
};


#endif
