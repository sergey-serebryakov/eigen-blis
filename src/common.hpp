#ifndef COMMON_HPP
#define COMMON_HPP

#include <sys/time.h>

#include "Eigen/Dense"

#define real_t    float
#define gemm_func cblas_sgemm
#define eigen_mat Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor | Eigen::AutoAlign>

unsigned long long get_time() {
  struct timeval now;
  gettimeofday (&now, 0);
  return  now.tv_usec + (unsigned long long)now.tv_sec * 1000000;
}

void benchmark_eigen(const int num_runs) {
  for (int dim=200; dim<10000; dim +=200) {
    const auto matrix_size = dim * dim;
    eigen_mat C = eigen_mat::Random(dim, dim),
              A = eigen_mat::Random(dim, dim),
              B = eigen_mat::Random(dim, dim);

    const auto start_time = get_time();
    for (int i=0; i<num_runs; ++i) {
      C.noalias() += A * B;
    }
    const double gemm_time   = static_cast<double>(get_time() - start_time) * 1.0e-6 / num_runs;
    const double gemm_gflops = 2.0d * dim * dim * dim / (1e9 * gemm_time);

    std::cout << dim << "\t\t" << gemm_gflops << std::endl;
  }
}

#endif
