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
  ByteBuffer buf(p, size);
  long timestamp = 0;
  short * payload = new short[PACKET_SIZE];
  buf.get_long(&timestamp);
  int payload_size = buf.remaining() / sizeof(short);
  buf.get_nshorts(payload, payload_size);
  return new MPacket(timestamp, payload, payload_size);
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << std::endl;
}
