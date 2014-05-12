#ifndef P2PIM_UDP_H
#define P2PIM_UDP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>


namespace p2pim_udp
{


	int initialize(std::string *args);

	int message_create(short type, const std::string *params, char *buffer);












}
namespace udp = p2pim_udp;
#endif

