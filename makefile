CC = g++
CFLAGS = -g -Wall

all: HFock
ARMADILLO_BASE=/user/poxonpea/armadillo-6.500.5
ARMADILLO_INCLUDE=$(ARMADILLO_BASE)/include

LIBS=-llapack -lblas -L$(ARMADILLO_BASE) 
INCLUDES=-Iinclude -I$(ARMADILLO_INCLUDE)          
#LIBS = -llapack -lblas -L/user/poxonpea/armadillo-6.500.5/ 

HFock: src/main.cpp lib/State.o lib/HartreeFock.o

	$(CC) $(CFLAGS) $(INCLUDES) -o bin/hfock src/main.cpp lib/State.o lib/HartreeFock.o $(LIBS)

lib/State.o: src/State.cpp include/State.hh
	$(CC) $(CFLAGS) $(INCLUDES) -c src/State.cpp -o lib/State.o $(LIBS)

lib/HartreeFock.o: include/State.hh src/HartreeFock.cpp include/HartreeFock.hh
	$(CC) $(CFLAGS) $(INCLUDES) -c src/HartreeFock.cpp -o lib/HartreeFock.o $(LIBS)

clean: 
	$(RM) lib/*.o bin/*
