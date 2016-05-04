#ifndef _MPACKET_H_
#define _MPACKET_H_

#define PACKET_SIZE 1024

#include <iostream>
#include <sstream>

class MPacket {
  public:
    MPacket(int time, char* p);
    char* get_payload();
    long get_timestamp();
    const char* pack();
    static MPacket* unpack(char * p, std::size_t n);

  private:
    char* payload;
    long timestamp;

};


#endif
