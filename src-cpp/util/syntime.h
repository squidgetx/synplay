#include <sys/time.h>

long get_millisecond_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}