#ifndef P2PIM_UDP_H
#define P2PIM_UDP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>


namespace p2pim_udp
{



	int message_create(short type, const std::string *params, char *buffer, int *buffer_size);



	void do_something(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor);









}
namespace udp = p2pim_udp;
#endif

