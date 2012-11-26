OBJS = main.o class.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

cache-sim : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cache-sim
	$(CC) workgen.cpp -o workgen	

main.o : class.o utility.h typedef.h
	$(CC) $(CFLAGS) main.cpp

cache.o : class.h typedef.h
	$(CC) $(CFLAGS) class.cpp
	
clean:
	\rm *.o
	
clear:
	\rm *.o

tar:
	tar cfv cache-sim-2.0.tar class.h class.cpp utility.h typedef.h main.cpp workgen.cpp Makefile README.md

