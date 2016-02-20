CC = g++
CFLAGS = -g -Wall

all: HFock

HFock: src/main.cpp lib/State.o lib/HartreeFock.o
	$(CC) $(CFLAGS) -Iinclude -o bin/hfock src/main.cpp lib/State.o lib/HartreeFock.o

lib/State.o: src/State.cpp include/State.hh
	$(CC) $(CFLAGS) -Iinclude -c src/State.cpp -o lib/State.o

lib/HartreeFock.o: include/State.hh src/HartreeFock.cpp include/HartreeFock.hh
	$(CC) $(CFLAGS) -Iinclude -c src/HartreeFock.cpp -o lib/HartreeFock.o

clean: 
	$(RM) lib/*.o bin/*
