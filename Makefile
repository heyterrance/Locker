CC=clang++
CFLAGS=-O3 -std=c++11

TARGET=lock

all: lock.cc util.h
	$(CC) $(CFLAGS) lock.cc -o $(TARGET)

clean:
	rm $(TARGET)
	
