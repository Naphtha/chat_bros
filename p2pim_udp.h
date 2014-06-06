#ifndef P2PIM_UDP_H
#define P2PIM_UDP_H

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
#include "message.h"
#include "p2pim_user.h"


namespace p2pim_udp
{


	// sets up connection using args or defaults
	// also allocates the UDP broadcast sockt and fd
	void initialize(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor,
					pollfd *file_descriptors);

	// creates a udp message for sending
	int message_create(int type, std::string *params, CNetworkMessage &mess);


	bool lookup_user(user &theUser, sockaddr_in *client_address);





}
namespace udp = p2pim_udp;
#endif

