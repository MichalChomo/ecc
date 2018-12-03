CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2
OBJS=file_loader.o

all: bms2A bms2B

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

bms2A: bms2A.cpp $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

bms2B: bms2B.cpp $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

clean:
	rm -f *.o bms2A bms2B
