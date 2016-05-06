# Directory Name
DIR=net

# Change these
NET_SOURCES := $(wildcard $(DIR)/*.cc)
NET_OBJECTS := $(patsubst %.cc, %.o, $(NET_SOURCES))

SOURCES += $(NET_SOURCES)
