#ifndef _TIME_PACKET_H_
#define _TIME_PACKET_H_

#include <iostream>
#include "asio.hpp"

#define NUM_TIMES (5)

typedef uint64_t mtime_t;

class TPacket {
  public:
    TPacket(mtime_t from_sent = 0, mtime_t to_recvd = 0, mtime_t to_sent = 0, 
        mtime_t from_recvd = 0, mtime_t offset = 0);
    asio::const_buffer pack() const;
    static TPacket* unpack(uint64_t* buf, std::size_t n);

    mtime_t from_sent;
    mtime_t to_recvd;
    mtime_t to_sent;
    mtime_t from_recvd;
    mtime_t offset;

  private:
};


#endif
