# Makefile for 'crawler' program
#
# By Rodrigo Vega Ayllon - October 2024

CS50 = ../libcs50
COMMON = ../common

OBJS = crawler.o
LIBS = $(CS50)/libcs50-given.a $(COMMON)/common.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
MAKE = make

crawler: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@	

$(COMMON)/common.a:
	$(MAKE) --directory=$(COMMON)

.PHONY: clean test

clean:
	rm -f core
	rm -f crawler *~ *.o
	$(MAKE) --directory=$(COMMON) clean

test:
	bash -v testing.sh
