#include "net/packet.h"
#include "net/time_packet.h"
#include "util/mpacket.h"

Packet::Packet() {
}

Packet::Packet(PacketType pt) : packet_type(pt) { 
}

PacketType Packet::get_type(){
  return packet_type;
}

Packet* Packet::unpack(uint8_t* buf, std::size_t size){
    uint8_t type = buf[0];
    switch (type) {
      case TIME:
        return TPacket::unpack(buf + 1,size - 1);
      case DATA:
        return MPacket::unpack(buf + 1,size - 1);
      default:
        std::cerr << "Data corruption detected: Invalid flag." << std::endl;
    }
}


