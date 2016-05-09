#include "asio.hpp"
#include "util/testing.h"

#include "net/time_packet.h"

void tpacket_rtt() {
  BEGIN;

  TPacket packet{1, 2, 3, 4};

  asio::const_buffer buf = packet.pack();
  const uint8_t * int_buf = asio::buffer_cast<const uint8_t *>(buf);
  TPacket *new_packet = TPacket::unpack(int_buf, asio::buffer_size(buf));

  EXPECT_EQ(packet.from_sent, new_packet->from_sent);
  EXPECT_EQ(packet.to_recvd, new_packet->to_recvd);
  EXPECT_EQ(packet.to_sent, new_packet->to_sent);
  EXPECT_EQ(packet.from_recvd, new_packet->from_recvd);
  EXPECT_EQ(packet.offset, new_packet->offset);

  delete new_packet;

  END;
}

int main() {
  tpacket_rtt();
}
