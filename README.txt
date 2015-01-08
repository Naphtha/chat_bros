Shawn Shojaie
996534619

Running the default make using 'make' will compile and link the objects to create the executable.

Running 'make run' will run the application with the args in the makefile, assuming a make has been run already.

Running 'make runtester' will run p2pimtester with some udp and tcp args consistent with make run.

Running 'make adduser' will prompt you to add a user to the auths file.

Running 'make anchor' is just a wrapper for p2pitrust.

'make clean' removes the executable and .o files.

While running pressing 'e' will exit the program gracefully.
Pressing 'l' will list current discovered peers.
Pressing 'c' will print out the list of discovered users and ask for a number. Enter a number of a user to send a tcp request for communication. This doesn't fully work yet.


Features:

Program supports all input syntax (-u, -tp, etc.).
Program broadcasts discovery correctly.
Program supports sending a unicast broadcast to external hosts.
Program can receive reply messages.
Program broadcasts closing message correctly.
Increases timeout when no clients are detected.
Program can receive messages through STDIN using noncanonical mode.
Program can list users that have been connected.
When started it prompts user for password and properly creates the public/private keys and modulus.
Program also creates nonce value and sends correctly encrypted AKRMs.
Program receives RAKMs and decrypts and parses the values. These values are confirmed to be correct when using a valid username password combo.
Program correctly unicasts and broadcasts the AKRM.
Program also uses timeout window for AKRMs like broadcast udp messages.
Program correctly sends all regular project 2 udp messages.