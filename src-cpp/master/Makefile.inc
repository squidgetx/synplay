DIR=master

MASTER_SOURCES := $(patsubst %, $(DIR)/%, msocket.cc master.cc main.cc)
MASTER_OBJECTS := $(patsubst %.cc, %.o, $(MASTER_SOURCES))

SOURCES += $(MASTER_SOURCES)
