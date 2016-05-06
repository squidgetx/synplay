#include "mpacket.h"

MPacket::MPacket(time_t time, int16_t* p, size_t ps) : timestamp(time), payload(p), payload_size(ps) {

}

size_t MPacket::get_payload_size() {
  return payload_size;
}

int16_t* MPacket::get_payload() {
  return payload;
}

time_t MPacket::get_timestamp() {
  return timestamp;
}

asio::const_buffer MPacket::pack() const {
  std::stringbuf buf;
  buf.sputn(reinterpret_cast<const uint8_t*>(&timestamp), sizeof(timestamp));
  buf.sputn(reinterpret_cast<const uint8_t*>(payload), payload_size);
  return asio::const_buffer(buf.str().data(), sizeof(timestamp) + payload_size);
}

MPacket * MPacket::unpack(uint8_t* p, std::size_t size) {
  ByteBuffer buf(p, size);
  time_t timestamp = 0;
  int16_t * payload = new int16_t[PACKET_SIZE];
  buf.get_long(&timestamp);
  size_t payload_size = buf.remaining() / sizeof(int16_t);
  buf.get_nshorts(payload, payload_size);
  return new MPacket(timestamp, payload, payload_size);
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << std::endl;
}
