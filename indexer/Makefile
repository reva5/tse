# Makefile for 'indexer' and 'indextest' programs
#
# By Rodrigo Vega Ayllon - October 2024

CS50 = ../libcs50
COMMON = ../common

LIBS = $(COMMON)/common.a $(CS50)/libcs50-given.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
MAKE = make

all: indexer indextest

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@	

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(COMMON)/common.a:
	$(MAKE) --directory=$(COMMON)

.PHONY: clean test

clean:
	rm -f core
	rm -f indexer indextest *~ *.o
	$(MAKE) --directory=$(COMMON) clean

test:
	bash -v testing.sh
