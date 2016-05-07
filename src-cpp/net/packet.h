#ifndef _PACKET_H_
#define _PACKET_H_

#include "asio.hpp"

typedef enum {
  TIME = 0,
  DATA = 1
} PacketType;

class Packet {
  public:
    Packet();
    Packet(PacketType pt);
    static Packet* unpack(uint8_t* buf, std::size_t size);
    PacketType get_type();

  protected:
    PacketType packet_type;
};

#endif
