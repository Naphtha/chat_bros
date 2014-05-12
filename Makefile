PARGS=-u steveener

all: retest

p2pim: main.o p2pim_udp.o
	g++ main.o p2pim_udp.o -o p2pim




main.o: main.cpp
	g++ -c -Wall main.cpp

udp.o: p2pim_udp.cpp
	g++ -c -Wall p2pim_udp.cpp

tcp.o: p2pim_tcp.cpp
	g++ -c -Wall p2pim_tcp.cpp




clean:
	rm -f *.o p2pim main

run:
	./p2pim $(PARGS)

retest:
	make clean
	make p2pim
	@echo "\n===========================\n"
	./p2pim $(PARGS)