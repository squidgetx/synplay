#include "time_packet.h"

TPacket::TPacket(){

}

asio::const_buffer TPacket::pack() const{

	uint64_t buf[NUM_TIMES];

	buf[0] = from_sent;
	buf[1] = to_recvd;
	buf[2] = to_sent;
	buf[3] = from_recvd;

	return asio::const_buffer(buf, NUM_TIMES);
}

static TPacket* unpack(uint64_t* buf, std::size_t n){
	return TPacket(buf[0],buf[1],buf[2],buf[3])
}
