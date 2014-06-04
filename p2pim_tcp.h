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
#include "message.h"
#include "p2pim_user.h"


namespace p2pim_tcp
{


	void initialize(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor,
					pollfd *file_descriptors);

	// for most messages
	int message_create(int type, CNetworkMessage &mess, std::string *params);

	int message_create(int type, CNetworkMessage &mess, const UserList &discoveredUsers);

	// for chat messages
	int message_create(int type, CNetworkMessage &mess, std::string theMessage);






}
namespace tcp = p2pim_tcp;
#endif

