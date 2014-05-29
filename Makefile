PARGS=-u notNitta

all: p2pim

p2pim: p2pim_udp.o p2pim_tcp.o main.o
	g++ main.o p2pim_udp.o p2pim_tcp.o -o p2pim




main.o: main.cpp
	g++ -c -Wall main.cpp

udp.o: p2pim_udp.cpp
	g++ -c -Wall p2pim_udp.cpp

tcp.o: p2pim_tcp.cpp
	g++ -c -Wall p2pim_tcp.cpp

message.o: message.cpp
	g++ -c -Wall message.cpp




clean:
	rm -f *.o p2pim main

run:
	./p2pim $(PARGS)

retest:
	make clean
	make p2pim
	@echo " "
	@echo "==========================="
	@echo " "
	./p2pim $(PARGS)