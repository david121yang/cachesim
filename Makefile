CC=g++
CFLAGS=-c -g -std=c++11 -Wall -Wextra -Wpedantic

csim: csim.o Cache.o
	$(CC) -o csim csim.o Cache.o

csim.o: csim.cpp Cache.h
	$(CC) $(CFLAGS) -c csim.cpp

Cache.o: Cache.cpp Cache.h
	$(CC) $(CFLAGS) -c Cache.cpp

clean:
	rm -f *.o csim
