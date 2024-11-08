# Makefile for 'querier' program
#
# By Rodrigo Vega Ayllon - October 2024

CS50 = ../libcs50
COMMON = ../common

LIBS = $(COMMON)/common.a $(CS50)/libcs50-given.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
MAKE = make

querier: querier.o tokens.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@	

tokens.o: tokens.h

$(COMMON)/common.a:
	$(MAKE) --directory=$(COMMON)

fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o fuzzquery

.PHONY: clean test

clean:
	rm -f core
	rm -f querier fuzzquery *~ *.o
	$(MAKE) --directory=$(COMMON) clean

test: fuzzquery
	bash -v testing.sh
