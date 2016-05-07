#include "time_packet.h"

TPacket::TPacket(mtime_t f_sent_, mtime_t t_recvd_, mtime_t t_sent_, mtime_t f_recvd_, mtime_t offset_) : from_sent(f_sent_), to_recvd(t_recvd_), to_sent(t_sent_), from_recvd(f_recvd_), offset(offset_), Packet::Packet(TIME) {

}

asio::const_buffer TPacket::pack() const{

  MutableByteBuffer buf;
  buf.write_uint8(TIME);
  buf.write_unsigned_long(from_sent);
  buf.write_unsigned_long(to_recvd);
  buf.write_unsigned_long(to_sent);
  buf.write_unsigned_long(from_recvd);
  buf.write_unsigned_long(offset);

  return asio::const_buffer(buf.data(), buf.size());
}

TPacket* TPacket::unpack(uint8_t* buf, std::size_t size){
  mtime_t *arr = reinterpret_cast<mtime_t *> (buf);
	return new TPacket(arr[0],arr[1],arr[2],arr[3],arr[4]);
}
