#include <cstdlib>
#include <iostream>
#include <ctime>

#include "common.hpp"

// http://eigen.tuxfamily.org/dox/TopicWritingEfficientProductExpression.html
// C[m, n] = A[m, k] * B[k, n] + C[m, n]

int main(int argc, char** argv) {
  const int num_runs = 10;
  std::srand(std::time(0));

  benchmark_eigen(num_runs);

  return EXIT_SUCCESS;
}
