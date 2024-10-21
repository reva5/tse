# Makefile for 'common' library
#
# By Rodrigo Vega Ayllon - October 2024

CS50 = ../libcs50

OBJS = pagedir.o
LIB = common.a

$(LIB): $(OBJS)
	ar -rc $(LIB) $(OBJS)

pagedir.o: pagedir.h $(CS50)/webpage.h

.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB)
	rm -f *~ *.o
