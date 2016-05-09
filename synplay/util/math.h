#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

#include <vector>
#include <deque>
#include <numeric>
#include <algorithm>

/**
 * Calculate the mean and stddeviation of a deque.
 *
 * first: mean
 * second stddev
 */
inline std::pair<double, double> calculate_mean_stddev(const std::deque<double>& dq) {
  double sum = std::accumulate(dq.begin(), dq.end(), 0.0);
  double mean = sum / dq.size();

  std::vector<double> diff(dq.size());
  std::transform(dq.begin(), dq.end(), diff.begin(), [mean](double x) { return x - mean; });
  double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / dq.size());
  return std::make_pair(mean, stdev);
}

#endif // UTIL_MATH_H_
