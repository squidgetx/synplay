#include "mpacket.h"

MPacket::MPacket(long time, short* p, int ps) : timestamp(time), payload(p), payload_size(ps) {

}

int MPacket::get_payload_size() {
  return payload_size;
}

short* MPacket::get_payload() {
  return payload;
}

long MPacket::get_timestamp() {
  return timestamp;
}

asio::const_buffer MPacket::pack() const {
  std::stringbuf buf;
  buf.sputn(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
  buf.sputn(reinterpret_cast<const char*>(payload), payload_size);
  return asio::const_buffer(buf.str().data(), sizeof(timestamp) + payload_size);
}

MPacket * MPacket::unpack(char* p, std::size_t size) {
  std::stringbuf buf;
  buf.str(p);
  long timestamp;
  short * payload = new short[PACKET_SIZE];
  buf.sgetn(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
  buf.sgetn(reinterpret_cast<char*>(payload), size - sizeof(timestamp));
  return new MPacket(timestamp, payload, size - sizeof(timestamp));
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << std::endl;
}
