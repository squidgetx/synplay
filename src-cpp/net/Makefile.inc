# Directory Name
DIR=net

# Change these
NET_SOURCES := $(patsubst %, $(DIR)/%, packet.cc time_packet.cc)
NET_OBJECTS := $(patsubst %.cc, %.o, $(NET_SOURCES))

SOURCES += $(NET_SOURCES)

bin/net_test: $(NET_OBJECTS) $(DIR)/test_time_packet.o util/mutable_byte_buffer.o util/mpacket.o util/bytebuffer.o
	$(CXX) $(CXXFLAGS) -o $@ $^
