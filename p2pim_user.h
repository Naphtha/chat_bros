#ifndef P2PIM_USER_H
#define P2PIM_USER_H

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <stdint.h>



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
		void printUsers();
		void addUser(const char *packet);
		void addUser(const user &theUser);
		user accessUser(int index);
		void removeUser(const char *packet);
		void removeUser(int index);
		bool empty();




};








#endif

