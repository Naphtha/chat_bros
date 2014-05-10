PARGS=-u bill

all:
	g++ -Wall main.cpp -o p2pim

clean:
	rm -f *.o p2pim

run:
	./p2pim $(PARGS)

retest:
	make clean
	make
	@echo "\n===========================\n"
	./p2pim $(PARGS)