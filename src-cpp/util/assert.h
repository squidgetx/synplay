// Common system assertions

#include <cstdlib>

/**
 * Confirm that the condition is true.
 */
inline void check_or_die(bool cond) {
  if (!cond)
    std::exit(EXIT_FAILURE);
}
