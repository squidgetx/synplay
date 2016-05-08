CXX=g++
OFLAG=-O3
CXXFLAGS=-g --std=c++11 -DASIO_STANDALONE -I"$(CURDIR)" -I"$(CURDIR)/lib/asio-1.10.6/include" $(OFLAG) $(DBG_FLAGS)
DBG_FLAGS=
LDFLAGS=-lportaudio -lsndfile

DIRS=client master net util
BIN_DIR=bin
SOURCES :=
BINS :=
OBJECTS = $(patsubst %.cc, %.o, $(SOURCES))


all:

%.cc : %.h

include $(patsubst %, %/Makefile.inc, $(DIRS))

BINS += $(BIN_DIR)/master
$(BIN_DIR)/master: $(MASTER_OBJECTS) $(NET_OBJECTS) $(UTIL_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

BINS += $(BIN_DIR)/client
$(BIN_DIR)/client: $(CLIENT_OBJECTS) $(NET_OBJECTS) $(UTIL_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: all
all: $(BINS)

.PHONY: clean
clean:
	rm `find . '-name' '*.o'`
	rm -f $(BINS)
