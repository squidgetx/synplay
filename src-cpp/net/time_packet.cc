#include "time_packet.h"

TPacket::TPacket(mtime_t f_sent_, mtime_t t_recvd_, mtime_t t_sent_, mtime_t f_recvd_, mtime_t offset_) : from_sent(f_sent_), to_recvd(t_recvd_), to_sent(t_sent_), from_recvd(f_recvd_), offset(offset_) {

}

asio::const_buffer TPacket::pack() const{

	mtime_t * buf = new mtime_t[NUM_TIMES];

	buf[0] = from_sent;
	buf[1] = to_recvd;
	buf[2] = to_sent;
	buf[3] = from_recvd;
	buf[4] = offset;

	return asio::const_buffer(buf, NUM_TIMES);
}

TPacket* TPacket::unpack(uint8_t* buf, std::size_t size){
  mtime_t *arr = reinterpret_cast<mtime_t *> (buf);
	return new TPacket(arr[0],arr[1],arr[2],arr[3],arr[4]);
}
