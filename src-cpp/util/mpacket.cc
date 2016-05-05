#include "mpacket.h"

MPacket::MPacket(int time, char* p, int ps) : timestamp(time), payload(p), payload_size(ps) {

}

int MPacket::get_payload_size() {
  return payload_size;
}

char* MPacket::get_payload() {
  return payload;
}

long MPacket::get_timestamp() {
  return timestamp;
}

const char * MPacket::pack() {
  std::stringbuf buf;
  buf.sputn(reinterpret_cast<char*>(timestamp), sizeof(long));
  buf.sputn(payload, payload_size);
  return buf.str().data();
}

MPacket * MPacket::unpack(char * p, std::size_t size) {
  std::stringbuf buf;
  buf.str(p);
  long timestamp;
  char * payload = new char[PACKET_SIZE];
  buf.sgetn(reinterpret_cast<char*>(&timestamp), sizeof(long));
  buf.sgetn(payload, size - sizeof(timestamp));
  return new MPacket(timestamp, payload, size - sizeof(timestamp));
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << std::endl;
}


