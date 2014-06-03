PARGS=-u notNitta -up 50222 -tp 50223

all: p2pim

p2pim: p2pim_udp.o p2pim_tcp.o message.o main.o
	g++ -o p2pim main.o p2pim_udp.o p2pim_tcp.o message.o




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