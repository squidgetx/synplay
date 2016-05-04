#include "mpacket.h"

MPacket::MPacket(int time, char* p) : timestamp(time), payload(p) {

}

char* MPacket::get_payload() {
  return payload;
}

long MPacket::get_timestamp() {
  return timestamp;
}

const char * MPacket::pack() {
  std::stringbuf buf;
  buf.sputn(reinterpret_cast<char*>(timestamp), 4);
  buf.sputn(payload, PACKET_SIZE);
  return buf.str().data();
}

MPacket * MPacket::unpack(char * p, std::size_t size) {
  std::stringbuf buf;
  buf.str(p);
  long timestamp;
  char * payload = new char[PACKET_SIZE];
  buf.sgetn(reinterpret_cast<char*>(&timestamp), 4);
  buf.sgetn(payload, size - sizeof(timestamp));
  return new MPacket(timestamp, payload);
}


