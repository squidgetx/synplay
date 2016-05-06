#ifndef _BYTEBUF_H
#define _BYTEBUF_H

#include <cstring>
#include <cstddef>
#include <cstdint>

// methods return the number of primitives successfully read
// from the buffer
class ByteBuffer {
  public:
    ByteBuffer(uint8_t* p, std::size_t s) : data_(p), size_(s), read_(0) {};
    int get_unsigned_long(uint64_t* dst);
    int get_short(int16_t* dst);
    std::size_t get_nshorts(int16_t* dst, std::size_t size);
    std::size_t remaining();
  protected:
    uint8_t* data_;
    std::size_t read_;
    std::size_t size_;
};

#endif
