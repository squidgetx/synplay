CXX=g++
OFLAG=-O3
CXXFLAGS=-g --std=c++11 -DASIO_STANDALONE -I"$(CURDIR)" -I"$(CURDIR)/lib/asio-1.10.6/include" $(OFLAG)

DIRS=client master util
BIN_DIR=bin
SOURCES :=
BINS :=
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))


all:

%.cc : %.h

include $(patsubst %, %/Makefile.inc, $(DIRS))

BINS += $(BIN_DIR)/master
$(BIN_DIR)/master: $(MASTER_OBJECTS) $(UTIL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

BINS += $(BIN_DIR)/client
$(BIN_DIR)/client: $(CLIENT_OBJECTS) $(UTIL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm `find . '-name' '*.o'`
	rm -f $(BINS)
