CC=gcc
CFLAGS=-pthread -Wall
CPLUSFLAGS = -lstdc++
all: clean
	touch Makefile
	$(CC) $(CFLAGS) Programm1/Client.cpp -o Client $(CPLUSFLAGS)
	$(CC) Programm2/Server.cpp -o Server $(CPLUSFLAGS)
clean: 
	rm -rf Client Server
