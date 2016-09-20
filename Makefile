BUILDDIR   := ./build
SRCDIR     := ./src
OPTDIR     := ./opt

LDFLAGS    := -L${OPTDIR}/lib -lblis
CXXFLAGS   := -I${OPTDIR}/include -I$(SRCDIR) -std=c++11 -fopenmp -O3 -ffast-math

SOURCES    := $(shell find ${SRCDIR} -name *.cpp)
OBJECTS    := $(patsubst ${SRCDIR}/%.cpp, ${BUILDDIR}/src/%.o, ${SOURCES})
EXECS      := $(patsubst ${BUILDDIR}/src/%.o, ${BUILDDIR}/%, ${OBJECTS})

all: ${EXECS}

${BUILDDIR}/eigen: ${BUILDDIR}/src/eigen.o
	${CXX} ${CXXFLAGS} $^ -o $@

${BUILDDIR}/blis: ${BUILDDIR}/src/blis.o
	${CXX} ${CXXFLAGS} $^ ${LDFLAGS} -o $@

${BUILDDIR}/eigen_blis: ${BUILDDIR}/src/eigen_blis.o
	${CXX} ${CXXFLAGS} $^ ${LDFLAGS} -o $@

${BUILDDIR}/src/%.o : ${SRCDIR}/%.cpp
	@mkdir -p $(@D)
	${CXX} ${CXXFLAGS} -MM -MT $@ $< > $(patsubst %.o, %.d, $@)
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	@rm -rf ${BUILDDIR}/*



-include $(OBJECTS:.o=.d)
