# Makefile for Unix builds

# Use the Intel Composer XE compiler. We can also use g++ with the Cilk Plus
# extensions
#CXX := icpc
CXX := g++

# Default to maximum optimization
OPT ?= -O3

CXXFLAGS := $(OPT) -I/home/adcastel/GLT/src/argobots -I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -DCORE 
# Use the best instructions available for this machine
#CXXFLAGS += -march=native

# We need to enable Cilk Plus if we're using g++
ifeq ($(CXX),g++)
#  CXXFLAGS += -fcilkplus -lcilkrts
endif

#SRC := driver.cpp karatsuba.cpp

all : karatsuba
	./karatsuba

karatsuba : $(SRC)
	$(CXX) $(CXXFLAGS) -o karatsuba $(SRC)

#driver.cpp : cilk_ticks.h karatsuba.h

#karatsuba.cpp : karatsuba.h temp_space.h

clean :
	rm -f karatsuba
