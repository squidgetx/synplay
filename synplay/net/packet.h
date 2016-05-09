#ifndef _PACKET_H_
#define _PACKET_H_

#include "asio.hpp"

enum struct PacketType {
  TIME = 0,
  DATA = 1
};

class Packet {
  public:
    Packet();
    Packet(PacketType pt);
    virtual asio::const_buffer pack() const = 0;
    static Packet* unpack(uint8_t* buf, std::size_t size);
    PacketType get_type();

  protected:
    PacketType packet_type;
};

#endif
