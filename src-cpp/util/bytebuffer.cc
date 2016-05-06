#include "bytebuffer.h"

int ByteBuffer::get_unsigned_long(uint64_t* dst) {
  if (remaining() < sizeof(*dst)) {
    return 0;
  }
  std::memcpy(dst, data_ + read_, sizeof(*dst));
  read_ += sizeof(*dst);
  return 1;
}

int ByteBuffer::get_short(int16_t* dst) {
  if (remaining() < sizeof(*dst)) {
    return 0;
  }
  std::memcpy(dst, data_ + read_, sizeof(*dst));
  read_ += sizeof(*dst);
  return 1;
}

std::size_t ByteBuffer::get_nshorts(int16_t* dst, std::size_t size) {
  for(int i = 0; i < size; i++) {
    int r = get_short(dst + i);
    if (r == 0) {
      return i;
    }
  }
  return size;
}

std::size_t ByteBuffer::remaining() {
  return size_ - read_;
}

std::size_t ByteBuffer::size() const {
  return size_;
}

