
ifeq ("$(wildcard ../config.mk)","")
  $(error No config.mk file has been found in the root folder of the project.))
endif

include ../config.mk

num_cores  ?= $(shell nproc)
blis_confname := ${BLIS_CONFNAME}

define BLIS_CONFIG
#define BLIS_CONFIG_H                   \n\
                                        \n\
#define BLIS_ENABLE_OPENMP              \n\
#define BLIS_ENABLE_CBLAS               \n\
#define BLIS_ENABLE_BLAS2BLIS           \n\
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE 32 \n\
                                        \n
endef
export BLIS_CONFIG

blis:
	set -e ;                                                                                 \
	git clone https://github.com/flame/blis ./blis ;                                         \
	cd ./blis ;                                                                              \
	git checkout 0.1.8 ;                                                                     \
	sed -i "s/#define BLIS_CONFIG_H/$$BLIS_CONFIG/" ./config/${blis_confname}/bli_config.h ; \
	./configure -p ../../opt ${blis_confname} ;                                              \
	make -j${num_cores} ;                                                                    \
	make install

eigen:
	hg clone https://bitbucket.org/eigen/eigen ;    \
	cd ./eigen ;                                    \
	hg up 3.3-beta2 ;                               \
	cp -r ./Eigen ../../opt/include/Eigen