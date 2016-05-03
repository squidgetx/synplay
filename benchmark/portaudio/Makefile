CC=gcc
CFLAGS=-pedantic -Wall -O3 -lportaudio

.PHONY: all
all:

listener: listener.o
	$(CC) $(CFLAGS) -o $@ $<

shouter: shouter.o
	$(CC) $(CFLAGS) -o $@ $<

all: listener shouter
