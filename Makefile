OBJS = main.o class.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

cache-sim : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cache-sim

main.o : class.o utility.h typedef.h
	$(CC) $(CFLAGS) main.cpp

cache.o : class.h typedef.h
	$(CC) $(CFLAGS) class.cpp

clear:
	\rm *.o

tar:
	tar cfv cache-sim-1.0.tar class.h class.cpp utility.h typedef.h main.cpp Makefile Readme.txt

