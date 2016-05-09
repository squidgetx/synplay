// Common system assertions

#include <cstdlib>

/**
 * Confirm that the condition is true.
 */
inline void check_or_die(bool cond, const char* err = "") {
  if (!cond)
    PANIC(err);
}

inline void PANIC(const char* err) {
  std::cerr << err << std::endl;
  std::exit(EXIT_FAILURE);
}
