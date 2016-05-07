#ifndef _TIME_PACKET_H_
#define _TIME_PACKET_H_

#include <iostream>
#include "asio.hpp"
#include "net/packet.h"
#include "util/mutable_byte_buffer.h"

#define NUM_TIMES (5)

typedef uint64_t mtime_t;
typedef int64_t mtime_offset_t;

typedef enum {
  INCOMPLETE = 0,
  COMPLETE = 1 // the offset field has been set.
} TPacketType;

class TPacket : public Packet {
  public:
    TPacket(TPacketType ptt = INCOMPLETE, mtime_t f_sent_ = 0, mtime_t t_recvd_ = 0, mtime_t t_sent_ = 0, mtime_t f_recvd_ = 0, mtime_offset_t offset_ = 0);
    asio::const_buffer pack() const;
    static TPacket* unpack(const uint8_t* buf, std::size_t size);

    void print();

    TPacketType tp_type;
    mtime_t from_sent;
    mtime_t to_recvd;
    mtime_t to_sent;
    mtime_t from_recvd;
    mtime_offset_t offset;

  private:
};


#endif
