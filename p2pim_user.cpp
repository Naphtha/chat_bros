#include "p2pim_user.h"



UserList::UserList(){

	if( !this->users.empty() ){
		this->users.clear();
	}
	

}

bool UserList::empty(){

	if( this->users.empty() ){
		return true;
	}
	else
		return false;
}

void UserList::printUsers(){

	std::cout << this->users.size() << " users found." << std::endl;

	for( int i = 0; i < this->users.size(); i++ ){

		std::cout << "User " << i << " " << this->users[i].username << "@"
		<< this->users[i].hostname << " on " << "UDP Port: "
		<< this->users[i].udp_port << " and TCP Port: " << this->users[i].tcp_port
		<< std::endl;

	}

	return;
}

void UserList::addUser(const char *packet){


	user theUser;
	uint16_t udp_port;
	uint16_t tcp_port;
	std::string hostname;
	std::string username;

	// if udp packet
	if( (0x01 == packet[5]) || (0x02 == packet[5]) || (0x03 == packet[5]) ){

		udp_port = packet[6];
		udp_port = udp_port << 8;
		udp_port += packet[7];

		tcp_port = packet[8];
		tcp_port = tcp_port << 8;
		tcp_port += packet[9];

		hostname = &(packet[10]);
		username = &(packet[10 + hostname.length() + 1]);

		// add parsed args to data structure
		theUser.udp_port = ntohs(udp_port);
		theUser.tcp_port = ntohs(tcp_port);
		theUser.hostname = hostname;
		theUser.username = username;

		this->users.push_back(theUser);

	}



	return;
}

void UserList::addUser(const user &theUser){

	this->users.push_back(theUser);
	return;
}

void UserList::removeUser(int index){

	this->users.erase( this->users.begin() + index );

	return;
}

user UserList::accessUser(int index){

	user blankUser;

	if( index < this->users.size() ){

		return this->users[index];
	}
	else{
		return blankUser;
	}

}


void UserList::removeUser(const char *packet){

	std::string hostname;
	std::string username;

	hostname = &(packet[10]);
	username = &(packet[10 + hostname.length() + 1]);

	for( int i = 0; i < this->users.size(); i++ ){

		if( this->users[i].hostname == hostname ){
			if( this->users[i].username == username ){
				this->users.erase( this->users.begin() + i );
				return;
			}
		}
	}


	return;

}


int UserList::size() const {

	return this->users.size();

}

