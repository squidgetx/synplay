DIR=master

MASTER_SOURCES = $(patsubst %, $(DIR)/%, master.cc)
MASTER_OBJECTS = $(patsubst %.cc, %.o, $(MASTER_SOURCES))

SOURCES += $(MASTER_SOURCES)
