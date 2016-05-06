#ifndef _TIME_PACKET_H_
#define _TIME_PACKET_H_

#include <iostream>
#include "asio.hpp"

#define NUM_TIMES (4)

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
    TPacket(mtime_t from_sent = 0, mtime_t to_recvd = 0, mtime_t to_sent = 0, mtime_t from_recvd = 0);
    asio::const_buffer pack() const;
    static TPacket* unpack(uint64_t* p, std::size_t n);
    ntp_time times;

  private:
};


#endif
