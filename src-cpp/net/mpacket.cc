#include "mpacket.h"

#include "util/mutable_byte_buffer.h"

MPacket::MPacket(mtime_t time, int16_t* p, size_t ps) : timestamp(time), 
  payload(p), payload_size(ps), Packet::Packet(PacketType::DATA) {

    pa_timestamp = 0;
    play_head = 0;
    
}

int MPacket::remaining() {
  return payload_size - play_head;
}

int16_t MPacket::get_int16_t() {
  int16_t ret = payload[play_head];
  play_head++;
  return ret;
}

PaTime MPacket::get_pa_timestamp() {
  return pa_timestamp;
}

void MPacket::set_pa_timestamp(PaTime pt) {
  pa_timestamp = pt;
}

size_t MPacket::get_payload_size() {
  return payload_size;
}

int16_t* MPacket::get_payload() {
  return payload;
}

mtime_t MPacket::get_timestamp() {
  return timestamp;
}

asio::const_buffer MPacket::pack() const {
  MutableByteBuffer buf;
  buf.write_uint8(static_cast<uint8_t>(PacketType::DATA));
  buf.write_unsigned_long(timestamp);
  buf.write_n_shorts(payload, payload_size);
  return asio::const_buffer(buf.data(), buf.size());
}

MPacket * MPacket::unpack(uint8_t* p, std::size_t size) {
  ByteBuffer buf(p, size);
  mtime_t timestamp = 0;
  int16_t * payload = new int16_t[BUFFER_SIZE];
  buf.get_unsigned_long(&timestamp);
  size_t payload_size = buf.remaining() / sizeof(int16_t);
  buf.get_nshorts(payload, payload_size);
  return new MPacket(timestamp, payload, payload_size);
}

void MPacket::print() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << " PaTime: " << pa_timestamp << ">" << std::endl;
}

void MPacket::print_all() {
  std::cout << "<MPacket timestamp: " << timestamp << " payload size: " << payload_size << " data: ";
  for(int i = 0; i < payload_size; i++) {
    printf("%i ", payload[i]);
  }
  std::cout << ">" << std::endl;
}

    
