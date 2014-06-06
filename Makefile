PARGS= -up 40667 -tp 40692

all: p2pim

p2pim: p2pim_udp.o p2pim_tcp.o message.o p2pim_user.o encryption.o main.o
	g++ -o p2pim main.o p2pim_udp.o p2pim_tcp.o message.o p2pim_user.o EncryptionLibrary.o




main.o: main.cpp
	g++ -c -Wall main.cpp

udp.o: p2pim_udp.cpp
	g++ -c -Wall p2pim_udp.cpp

tcp.o: p2pim_tcp.cpp
	g++ -c -Wall p2pim_tcp.cpp

message.o: message.cpp
	g++ -c -Wall message.cpp

user.o: p2pim_user.cpp
	g++ -c -Wall p2pim_user.cpp

encryption.o: EncryptionLibrary.cpp
	g++ -c -Wall EncryptionLibrary.cpp




clean:
	rm -f *.o p2pim main

run:
	./p2pim $(PARGS)

runtester:
	./p2pimtester -up 40667 -tp 40692

retest:
	make clean
	make p2pim
	@echo " "
	@echo "==========================="
	@echo " "
	./p2pim $(PARGS)