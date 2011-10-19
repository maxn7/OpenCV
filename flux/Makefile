# Makefile
# pour un projet opencv

CC=g++
CFLAGS=-Wall 
LDFLAGS=-L /usr/include/opencv/ -lhighgui -lcv -lm -lcvaux

all: clean output

output: flux.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o output flux.cpp 

clean:
	rm -rf *.o 
