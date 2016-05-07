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
    uint8_t type = *(buf++);
    switch (type) {
      case TIME:
        return TPacket::unpack(buf,size);
      case DATA:
        return MPacket::unpack(buf,size);
      defaut:
        std::cerr << "Data corruption detected: Invalid flag." << std::endl;
    }
}


