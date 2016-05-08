#ifndef _MPACKET_H_
#define _MPACKET_H_

#define PACKET_SIZE 1024

#include <iostream>
#include <sstream>
#include "asio.hpp"
#include "util/bytebuffer.h"
#include "net/packet.h"
#include <portaudio.h>

class MPacket : public Packet {
  public:
    MPacket(time_t time, int16_t* p, size_t psize);
    int16_t* get_payload();
    size_t get_payload_size();
    time_t get_timestamp();
    PaTime get_pa_timestamp();
    void set_pa_timestamp(PaTime pt);
    void print();
    void print_all();
    asio::const_buffer pack() const;
    static MPacket* unpack(uint8_t * p, std::size_t n);

  private:
    int16_t* payload;
    size_t payload_size;
    time_t timestamp;
    PaTime pa_timestamp;
};


#endif
