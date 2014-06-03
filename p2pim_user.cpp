#include "p2pim_user.h"



UserList::UserList(){

}

void UserList::addPacket(const char *packet){


	user theUser;
	uint16_t udp_port;
	uint16_t tcp_port;
	std::string hostname;
	std::string username;



	if( packet[5] == 0x01 ){

		udp_port = packet[6];
		udp_port = udp_port << 8;
		udp_port += packet[7];

		tcp_port = packet[8];
		tcp_port = tcp_port << 8;
		tcp_port += packet[9];

		hostname = &(packet[10]);
		username = &(packet[10 + hostname.length()]);

		std::cout << "udp_port = " << udp_port << std::endl;
		std::cout << "tcp_port = " << tcp_port << std::endl;
		std::cout << "hostname = " << hostname << std::endl;
		std::cout << "username = " << username << std::endl;


	}




}
