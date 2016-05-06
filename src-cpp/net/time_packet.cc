#include "time_packet.h"

TPacket::TPacket(){

}

ntp_time TPacket::get_times(){
	return times;
}

asio::const_buffer TPacket::pack() const{

	uint64_t buf[NUM_TIMES];

	buf[0] = times.from_sent;
	buf[1] = times.to_recvd;
	buf[2] = times.to_sent;
	buf[3] = times.from_recvd;

	return asio::const_buffer(buf, NUM_TIMES);
}

static TPacket* unpack(uint64_t* p, std::size_t n){
	return TPacket(buf[0],buf[1],buf[2],buf[3])
}
