#include <cstdlib>
#include <iostream>
#include <memory>
#include <algorithm>
#include <ctime>

#include "blis/cblas.h"

#include "common.hpp"


// C[m, n] = A[m, k] * B[k, n] + C[m, n]
int main(int argc, char** argv) {
  const int num_runs = 10;
  std::srand(std::time(0));

  for (int dim=200; dim<10000; dim +=200) {
    const auto matrix_size = dim * dim;
    std::unique_ptr<real_t[]> A(new real_t[matrix_size]),
                              B(new real_t[matrix_size]),
                              C(new real_t[matrix_size]);
    for (int i=0; i<matrix_size; ++i) {
      A[i] = static_cast<real_t>(rand()) / RAND_MAX;
      B[i] = static_cast<real_t>(rand()) / RAND_MAX;
      C[i] = static_cast<real_t>(rand()) / RAND_MAX;
    }

    const auto start_time = get_time();
    for (int i=0; i<num_runs; ++i) {
      gemm_func(CblasRowMajor, CblasNoTrans, CblasNoTrans, dim, dim, dim, 1, A.get(), dim, B.get(), dim, 1, C.get(), dim);
    }
    const double gemm_time   = static_cast<double>(get_time() - start_time) * 1.0e-6 / num_runs;
    const double gemm_gflops = 2.0d * dim * dim * dim / (1e9 * gemm_time);
    
    std::cout << dim << "\t\t" << gemm_gflops << std::endl;
  }
  
  return EXIT_SUCCESS;
}
