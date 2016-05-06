#ifndef _BYTEBUF_H
#define _BYTEBUF_H

#include <cstring>
// methods return the number of primitives successfully read
// from the buffer
//
class ByteBuffer {
  public:
    ByteBuffer(char* p, int s) : data(p), size(s), read(0) {};
    int get_long(long* dst);
    int get_short(short* dst);
    int get_nshorts(short* dst, int size);
    int remaining();
  private:
    char* data;
    int read;
    int size;
};

#endif
