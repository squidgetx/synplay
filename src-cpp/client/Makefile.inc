# Directory Name
DIR=client

# Change these
CLIENT_SOURCES = $(wildcard $(DIR)/*.cc)
CLIENT_OBJECTS = $(patsubst %.cc, %.o, $(CLIENT_SOURCES))

SOURCES += $(CLIENT_SOURCES)
