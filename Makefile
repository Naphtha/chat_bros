PARGS=-u steven -u bill -up 5040 -tp 5041 -dt 3 -dm 26

all: retest

p2pim: main.o
	g++ main.o -o p2pim


main.o: main.cpp
	g++ -c -Wall main.cpp


clean:
	rm -f *.o p2pim main

run:
	./p2pim $(PARGS)

retest:
	make clean
	make p2pim
	@echo "\n===========================\n"
	./p2pim $(PARGS)