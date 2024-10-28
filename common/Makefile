# Makefile for 'common' library
#
# By Rodrigo Vega Ayllon - October 2024

CS50 = ../libcs50

OBJS = pagedir.o index.o word.o
LIB = common.a

$(LIB): $(OBJS)
	ar -rc $(LIB) $(OBJS)

pagedir.o: pagedir.h $(CS50)/webpage.h $(CS50)/file.h $(CS50)/mem.h
index.o: index.h $(CS50)/hashtable.h $(CS50)/counters.h $(CS50)/file.h $(CS50)/mem.h
word.o: word.h

.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB)
	rm -f *~ *.o
