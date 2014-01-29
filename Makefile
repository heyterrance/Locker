CC=clang++
CFLAGS=-O3 -std=c++11

TARGET=lock

all: lock.o util.h main.cc
	$(CC) $(CFLAGS) main.cc lock.o -o $(TARGET)

lock.o: lock.cc lock.h util.h
	$(CC) $(CFLAGS) -c lock.cc -o lock.o

clean:
	rm $(TARGET) lock.o
