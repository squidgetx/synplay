#ifndef _RINGBUF_H_
#define _RINGBUF_H_

template <typename T>
class RingBuffer {
  public:
    RingBuffer(int cap) : capacity(cap) {
      read_head = 0;
      write_head = 0;
      data = new T[cap];
    }
    int putn(T* data, int n);
    int put(T datum);
    int getn(T* data, int n);
    T get();
  private: 
    T* data;
    int read_head;
    int write_head;
    int capacity;
};

template <class T>
int RingBuffer<T>::put(T datum) {
  if (write_head + 1 % capacity == read_head) {
    return 0;
  }
  data[write_head] = datum;
  write_head++;
  if (write_head == capacity) 
    write_head = 0;
  return 1;
}

template <class T>
int RingBuffer<T>::putn(T* data, int n) {
  for(int i = 0; i < n; i++) {
    int result = put(data[i]);
    if (result == 0) {
      return i;
    }
  }
  return n;
}

template <class T>
T RingBuffer<T>::get() {
  if (read_head == write_head) {
    return NULL;
  }
  T result = data[read_head];
  read_head++;
  if (read_head == capacity) {
    read_head = 0;
  }
  return result;
}

template <class T>
int RingBuffer<T>::getn(T* buffer, int n) {
  for(int i = 0; i< n; i++) {
    buffer[i] = get();
    if (buffer[i] == NULL) {
      return i;
    }
  }
  return n;
}

#endif
