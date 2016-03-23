# Makefile for Unix builds

# Use the Intel Composer XE compiler. We can also use g++ with the Cilk Plus
# extensions
CXX := icpc
#CXX := gcc

# Default to maximum optimization
OPT ?= -O3

CXXFLAGS := $(OPT) 
# Use the best instructions available for this machine
CXXFLAGS += -march=native

# We need to enable Cilk Plus if we're using g++
ifeq ($(CXX),g++)
  CXXFLAGS += -fcilkplus -lcilkrts
endif

SRC := driver.cpp karatsuba.cpp

all : karatsuba
	./karatsuba

karatsuba : $(SRC)
	$(CXX) $(CXXFLAGS) -o karatsuba $(SRC)

driver.cpp : cilk_ticks.h karatsuba.h

karatsuba.cpp : karatsuba.h temp_space.h

clean :
	rm -f karatsuba
