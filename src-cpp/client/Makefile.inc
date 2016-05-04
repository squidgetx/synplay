# Directory Name
DIR=client

# Change these
CLIENT_SOURCES = $(patsubst %, $(DIR)/%, client.cc)
CLIENT_OBJECTS = $(patsubst %.cc, %.o, $(CLIENT_SOURCES))

SOURCES += $(CLIENT_SOURCES)
BINS += client

#$(DIR)/LockRequestLinkedList.o : $(DIR)/TLinkedList.h
