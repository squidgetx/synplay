CXX=g++
OFLAG=-O3
CXXFLAGS=-pthread -g --std=c++11 -I$(CURDIR) $(OFLAG)

DIRS=
SOURCES :=
BINS :=
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))

all:

include $(patsubst %, %/Makefile.inc, $(DIRS))

.PHONY: all
all: $(OBJECTS) $(BINS)

.PHONY: clean
clean:
	rm `find . '-name' '*.o'`
	rm -f $(BINS)
