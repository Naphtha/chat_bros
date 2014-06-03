#ifndef P2PIM_USER_H
#define P2PIM_USER_H

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string.h>



struct user{
	uint16_t tcp_port;
	uint16_t udp_port;
	std::string hostname;
	std::string username;
};


class UserList{

	protected:
		std::vector< user > users;

	public:
		UserList();
		void addPacket(const char *packet);





};








#endif

