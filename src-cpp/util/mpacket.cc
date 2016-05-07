#include "mpacket.h"

#include "util/mutable_byte_buffer.h"

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
  MutableByteBuffer buf;
  buf.write_uint8(DATA);
  buf.write_unsigned_long(timestamp);
  buf.write_n_shorts(payload, payload_size);
  return asio::const_buffer(buf.data(), buf.size());
}

MPacket * MPacket::unpack(uint8_t* p, std::size_t size) {
  std::cerr << "mpacket::unpack" << std::endl;
  ByteBuffer buf(p, size);
  uint64_t timestamp = 0;
  int16_t * payload = new int16_t[PACKET_SIZE];
  buf.get_unsigned_long(&timestamp);
  size_t payload_size = buf.remaining() / sizeof(int16_t);
  buf.get_nshorts(payload, payload_size);
  return new MPacket(timestamp, payload, payload_size);
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << ">" << std::endl;
}

void MPacket::print_all() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << " data: ";
  for(int i = 0; i < payload_size; i++) {
    printf("%i ", payload[i]);
  }
  std::cout << ">" << std::endl;
}

    
