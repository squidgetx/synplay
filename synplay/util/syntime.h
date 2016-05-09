#ifndef SYNTIME_H
#define SYNTIME_H

#include <sys/time.h>

time_t get_millisecond_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

#endif
