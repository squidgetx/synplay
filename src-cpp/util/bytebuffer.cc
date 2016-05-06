#include "bytebuffer.h"

int ByteBuffer::get_long(long* dst) {
  if (remaining() < sizeof(long)) {
    return 0;
  }
  std::memcpy(dst, data+read, sizeof(long));
  read += sizeof(long);
  return 1;
}

int ByteBuffer::get_short(short* dst) {
  if (remaining() < sizeof(short)) {
    return 0;
  }
  std::memcpy(dst, data+read, sizeof(short));
  read += sizeof(short);
  return 1;
}

int ByteBuffer::get_nshorts(short* dst, int size) {
  for(int i = 0; i < size; i++) {
    int r = get_short(dst + i);
    if (r == 0) {
      return i;
    }
  }
  return size;
}

int ByteBuffer::remaining() {
  return size - read;
}


