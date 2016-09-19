# Accelerating Eigen linear algebra library with BLIS (BLAS) on CPUs

### Introduction
[Eigen](http://eigen.tuxfamily.org/) is a C++ template-based linear algebra library. In order to use it, no compilation is requried: just check out the latest version and specify include paths. Eigen uses a number of optimization techniques to make things fast: parallelization with OpenMP, advanced vector extensions (AVX), smart expression templates and many others.

Linear algebra is an important component of many HPC applications (for instance, the process of training deep neural networks may end up multiplying matrices up to 90% of entire training time). There is a de-facto standard for low-level vector-matrix operations called [BLAS](https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms). Usually, libraries implementing BLAS interface are highly optimized for various types of hardware and need to be compiled and built on target machines.

Eigen historically supported only Intel's MKL library to delegate some of its computations to a highly optimzied ans specialzied for a particular hardware code. However, starting from version 3.3 Eigen can delegate these computations to any library implementing BLAS API (see [this](http://eigen.tuxfamily.org/dox-devel/TopicUsingBlasLapack.html) page for details)).

There are many libraries providing BLAS interface. From my experience, I found [BLIS](https://github.com/flame/blis) to be very convinient and fast. This project aims at demonstrating that a particular operation - general matrix-matrix multiplication - can be accelerated by allowing Eigen to delegate it to a BLAS library (BLIS version 0.1.8 in this particular case).
### Using Eigen with BLAS library
In order to take advantage of hardware-optimized linear algebra library, Eigen requires that a macro EIGEN_USE_BLAS is defined before including Eigen headers
```c
#define EIGEN_USE_BLAS
#include "Eigen/Dense"
```
and a program is linked against corresponding library. That's basically all changes!
### Running examples in this project
