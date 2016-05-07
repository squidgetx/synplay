#include "time_packet.h"

TPacket::TPacket(mtime_t f_sent_, mtime_t t_recvd_, mtime_t t_sent_, mtime_t f_recvd_, mtime_offset_t offset_) : from_sent(f_sent_), to_recvd(t_recvd_), to_sent(t_sent_), from_recvd(f_recvd_), offset(offset_), Packet::Packet(TIME) {

}

asio::const_buffer TPacket::pack() const{
  MutableByteBuffer buf;
  buf.write_uint8(TIME);
  buf.write_unsigned_long(from_sent);
  buf.write_unsigned_long(to_recvd);
  buf.write_unsigned_long(to_sent);
  buf.write_unsigned_long(from_recvd);
  buf.write_signed_long(offset);
  return asio::const_buffer(buf.data(), buf.size());
}

TPacket* TPacket::unpack(const uint8_t* buf, std::size_t size){
  const mtime_t *u_arr = reinterpret_cast<const mtime_t *> (buf);
  const mtime_offset_t *s_arr = reinterpret_cast<const mtime_offset_t *> (buf + 4 * sizeof(mtime_t));

  return new TPacket(u_arr[0],u_arr[1],u_arr[2],u_arr[3],s_arr[0]);
}

void TPacket::print() {
  std::cerr << "Timesync: from_sent: " << from_sent << " to_recvd: " << to_recvd << " to_sent: " << to_sent << " from_recvd: " << from_recvd << " offset: " << offset << std::endl;
}
