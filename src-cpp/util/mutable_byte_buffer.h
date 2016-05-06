#ifndef _MUTABLEBYTEBUF_H
#define _MUTABLEBYTEBUF_H

#include <cstring>
#include <cstddef>
#include <cstdint>

#include "util/bytebuffer.h"

// methods return the number of primitives successfully read
// from the buffer
//
class MutableByteBuffer : public ByteBuffer {
  public:
    MutableByteBuffer(std::size_t initial_size = 64);
    MutableByteBuffer(const MutableByteBuffer &other);
    ~MutableByteBuffer();

    void write_unsigned_long(uint64_t val);
    void write_short(int16_t val);
    void write_n_shorts(const int16_t *vals, std::size_t n);
  private:
    void increase_size();
    void write(const uint8_t *data, std::size_t nbytes);

    std::size_t write_;
};

#endif
