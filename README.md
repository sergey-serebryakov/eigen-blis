# Accelerating Eigen linear algebra library with BLIS (BLAS) on CPUs

### Introduction
[Eigen](http://eigen.tuxfamily.org/) is a C++ template-based linear algebra library. In order to use it, no compilation is requried: just check out the latest version and specify include paths for your project. Eigen uses a number of optimization techniques to make things fast: parallelization with OpenMP, advanced vector extensions (AVX), smart expression templates and many others.

Linear algebra is an important component of many HPC applications (for instance, the process of training deep neural networks may end up multiplying matrices up to 90% of entire training time). There is a de-facto standard for low-level vector-matrix operations called [BLAS](https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms). Usually, libraries implementing BLAS interface are highly optimized for various types of hardware and need to be compiled and built on target machines.

Eigen historically supported only Intel's MKL library to delegate some of its computations to a highly optimzied ans specialzied for a particular hardware code. However, starting from version 3.3 Eigen can delegate these computations to any library implementing BLAS API (see [this](http://eigen.tuxfamily.org/dox-devel/TopicUsingBlasLapack.html) page for details)).

There are many libraries providing BLAS interface. From my experience, I found [BLIS](https://github.com/flame/blis) to be very convinient and fast. This project aims at demonstrating that a particular operation - general matrix-matrix multiplication (xgemm) - can be accelerated by allowing Eigen to delegate it to a BLAS library (BLIS version 0.1.8 in this particular case).
### Using Eigen with BLAS library
In order to take advantage of a hardware-optimized linear algebra library, Eigen requires that a macro EIGEN_USE_BLAS is defined before including Eigen headers
```c
#define EIGEN_USE_BLAS
#include "Eigen/Dense"
```
and a program is linked against corresponding library. That's basically all!
### Running examples in this project
Get this project and create initial configuration:
```shell
git clone https://github.com/sergey-serebryakov/eigen-blis.git
cd ./eigen-blis
cp ./config.mk.template ./config.mk
```
Previous versions of BLIS require that a CPU configuration is provided (go [here](https://github.com/flame/blis/tree/0.1.8/config) to see which configurations are available). Edit the BLIS_CONFNAME variable in config .mk file to specify a desired configuration. It is very important to select a correct configuration at this step. Once configuration is specified, it is time to compile BLIS and get Eigen (at this step you may need to adjust your environmental settings (proxy settings) or install additional dependencies (mercurial)):
```shell
cd ./third_party
make eigen
make blis
cd ..
```

Compile and build the project:
```shell
make
```
As a result, there will be three executable files in ./build directory:
  * **blis**: BLIS-based matrix multiplication
  * **eigen**: Eigen-based matrix multiplication
  * **eigen_blis**: Eigen + BLIS-based matrix multiplication

Before running examples, an environment must be properly set. To configure Eigen, you need to figure out the number of physical cores your CPU has (do not take into accout Hyper-Threading!). Then export OMP_NUM_THREADS variable (15 cores here is for an Intel's Ivy Bridge family server processor (E7-4890 v2)):
 ```shell
 export OMP_NUM_THREADS=15
 ```
There are multiple ways how BLIS can be configured in [multithreading mode](https://github.com/flame/blis/wiki/Multithreading). In most of my experiments, I configure only BLIS_JC_NT and BLIS_IC_NT variables (this example is for the same 15-core processor):
 ```shell
 export BLIS_JC_NT=3
 export BLIS_IC_NT=5
 ```
 
Once it is done, go to buid directory and run binaries. Each file will iterate over squared matrix dimensions in the range 200 - 10000 with step 200 and will multiply matrices that will be translated by Eigen to exactly one gemm call (C = A * B + C) (see Eigen [documentation](http://eigen.tuxfamily.org/dox/TopicWritingEfficientProductExpression.html) how to write efficient matrix expressions).

If you run examples on multi-CPU server, do not forget to bind the app to one cpu with either **taskset** or **numactl** utilities. For example, to run the application on the first CPU on a multi-CPU machine, following commands may be used (taskset here is for a 15-core CPU):
  ```shell
 taskset -c 0-14 ./blis
 numactl --cpunodebind=0 --membind=0 ./blis
 ```
 You may want to try other CPUs since CPU 0 is used by OS.
 
### Experimental results
 
##### 1. CPU: E7-4890 v2 @ 2.8 (Ivy Bridge), GCC: 4.8.5, OS: Red Hat 7.2
The machine is a server with 4 Intel's E7-4890 v2 @ 2.8 processors (theoretical peak for single precision floating point numbers is 672 GFLOPs). BLIS was configured with OpenMP and SandyBridge kernel. The following script was used to run experiments:
  ```shell
 export OMP_NUM_THREADS=15
 export BLIS_JC_NT=3
 export BLIS_IC_NT=5
 nuamctl --cpunodebind=2 --membind=2 ./blis
 nuamctl --cpunodebind=2 --membind=2 ./eigen_blis
 nuamctl --cpunodebind=2 --membind=2 ./eigen
 ```
 The following chart depicts achieved GFLOPs for different matrix dimensions:
 
![sgemm E7-4890 v2](https://docs.google.com/uc?id=0B9MJrpMhxr32MVZ4WXVOaGNUUVU)
