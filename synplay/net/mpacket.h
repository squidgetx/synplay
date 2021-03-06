#ifndef _MPACKET_H_
#define _MPACKET_H_

#include <iostream>
#include <sstream>
#include "asio.hpp"
#include "util/bytebuffer.h"
#include "net/packet.h"
#include <portaudio.h>

// for the mtime_t data type
#include "net/time_packet.h"

class MPacket : public Packet {
  public:
    MPacket(mtime_t time, int16_t* p, size_t psize);
    int16_t* get_payload();
    size_t get_payload_size();
    mtime_t get_timestamp();
    void print();
    void print_all();
    asio::const_buffer pack() const;
    static MPacket* unpack(uint8_t * p, std::size_t n);

    /* Methods to control playback on the client side */
    void set_pa_timestamp(PaTime pt);
    PaTime get_pa_timestamp();

    // Pull an int16_t from the packet payload buffer
    int16_t get_int16_t();

    // How many int16_ts are left in the packet payload buffer
    int remaining();

  private:
    int16_t* payload;
    size_t payload_size;
    PaTime pa_timestamp;
    int play_head;
    mtime_t timestamp;
};


#endif
