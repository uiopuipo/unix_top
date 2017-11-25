# Makefile

CC=gcc
CFLAGS=
OBJS=main.o option.o
LIBS=
all:	add

add:	$(OBJS)
	$(CC) $(CFLAGS) -o myTop $(OBJS) $(LIBS)
	
main.o:	main.c
	$(CC) $(CFLAGS) -c main.c -l stdhdr.h
option.o: option.c
	$(CC) $(CFLAGS) -c option.c -l stdhdr.h

clean:
	rm -f $(OBJS) add core

