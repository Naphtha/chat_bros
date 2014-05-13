Shawn Shojaie
996534619

Running the default make using 'make' will compile and link the objects to create the executable.

Running 'make run' will run the application with the args in the makefile, assuming a make has been run already.

'make clean' removes the executable and .o files.

While running pressing 'e' will exit the program gracefully.
Pressing 'l' will list current discovered peers.



Features:

Program supports all input syntax (-u, -tp, etc.).
Program broadcasts discovery correctly.
Program supports sending a unicast broadcast to external hosts.
Program can receive reply messages.
Program broadcasts closing message correctly.
Increases timeout when no clients are detected.
Program can receive messages through STDIN using noncanonical mode.
Contains some structure code to begin adding clients and sending TCP messages.