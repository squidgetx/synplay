#ifndef _MPACKET_H_
#define _MPACKET_H_

#define PACKET_SIZE 1024

#include <iostream>
#include <sstream>

class MPacket {
  public:
    MPacket(int time, char* p, int psize);
    char* get_payload();
    int get_payload_size();
    long get_timestamp();
    void print();
    const char* pack();
    static MPacket* unpack(char * p, std::size_t n);

  private:
    char* payload;
    int payload_size;
    long timestamp;

};


#endif
