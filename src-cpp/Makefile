CXX=g++
OFLAG=-O3
CXXFLAGS=-g --std=c++11 -I$(CURDIR) $(OFLAG)

DIRS=
SOURCES :=
BINS :=
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))


all:

%.cc : %.h

include $(patsubst %, %/Makefile.inc, $(DIRS))

BINS += master
master: $(MASTER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $<

BINS += client
client: $(CLIENT_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm `find . '-name' '*.o'`
	rm -f $(BINS)
