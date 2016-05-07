#ifndef _TIME_PACKET_H_
#define _TIME_PACKET_H_

#include <iostream>
#include "asio.hpp"
#include "net/packet.h"
#include "util/mutable_byte_buffer.h"

#define NUM_TIMES (5)

typedef uint64_t mtime_t;

class TPacket : public Packet {
  public:
    TPacket(mtime_t f_sent_ = 0, mtime_t t_recvd_ = 0, mtime_t t_sent_ = 0, mtime_t f_recvd_ = 0, mtime_t offset_ = 0); 
    asio::const_buffer pack() const;
    static TPacket* unpack(uint8_t* buf, std::size_t size);

    mtime_t from_sent;
    mtime_t to_recvd;
    mtime_t to_sent;
    mtime_t from_recvd;
    mtime_t offset;

  private:
};


#endif
