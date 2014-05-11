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

namespace p2pim_udp
{



	int message_create(int type, const std::string *params, char *buffer);












}
namespace udp = p2pim_udp;
#endif

