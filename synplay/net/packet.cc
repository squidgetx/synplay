#include "net/packet.h"
#include "net/time_packet.h"
#include "net/mpacket.h"

Packet::Packet() {
}

Packet::Packet(PacketType pt) : packet_type(pt) {

}

PacketType Packet::get_type(){
  return packet_type;
}

Packet* Packet::unpack(uint8_t* buf, std::size_t size){
    PacketType type = static_cast<PacketType>(buf[0]);
    switch (type) {
      case PacketType::TIME:
        return TPacket::unpack(buf + 1,size - 1);
      case PacketType::DATA:
        return MPacket::unpack(buf + 1,size - 1);
      default:
        std::cerr << "Data corruption detected: Invalid flag." << std::endl;
        return nullptr;
    }
}

