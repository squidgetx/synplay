#ifndef ASSERT_H
#define ASSERT_H
// Common system assertions

#include <cstdlib>

inline void PANIC(const char* err) {
  std::cerr << err << std::endl;
  std::exit(EXIT_FAILURE);
}

/**
 * Confirm that the condition is true.
 */
inline void check_or_die(bool cond, const char* err = "") {
  if (!cond)
    PANIC(err);
}

#endif
