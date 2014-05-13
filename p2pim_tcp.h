#ifndef P2PIM_TCP_H
#define P2PIM_TCP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>


namespace p2pim_tcp
{


	void initialize(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor,
					pollfd *file_descriptors);

	// message create for generic functions
	// creates messages 04, 05, 06, 07, 0A
	int messsage_create(short type, std::string *params, char *buffer, int *buffer_size);








}
namespace tcp = p2pim_tcp;
#endif

