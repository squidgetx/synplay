# Directory Name
DIR=net

# Change these
NET_SOURCES := $(wildcard $(DIR)/*.cc)
UTIL_OBJECTS := $(patsubst %.cc, %.o, $(NET_SOURCES))

SOURCES += $(NET_SOURCES)
