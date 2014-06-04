#include "p2pim_tcp.h"

void tcp::initialize(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor,
					pollfd *file_descriptors){

	int port_number;

    *socket_file_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(0 > *socket_file_descriptor){
        perror("Error opening socket");
    }


    port_number = atoi( args[2].c_str() );


    bzero((char *) server_address, sizeof(*server_address));

    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = htons(port_number);

    if(0 > bind(*socket_file_descriptor, (struct sockaddr *)server_address, sizeof(*server_address))){ 
        perror("Error binding TCP socket: ");
    }

    listen(*socket_file_descriptor, 5);

    file_descriptors[1].fd = *socket_file_descriptor;
    file_descriptors[1].events = POLLIN;

    return;
}



int tcp::message_create(int type, CNetworkMessage &mess, std::string *params){


	std::string username;

	mess.AppendStringWithoutNULL("P2PI");
	mess.AppendUInt16( (uint16_t) type);

	// message is an establish communications
	if( 0x04 == type ){

		username = params[0];
		mess.AppendString(username);
	}

	return true;

}


// only to be used with userlist reply messages
int tcp::message_create(int type, CNetworkMessage &mess, const UserList &discoveredUsers){

	if( 0x08 != type ){
		return false;
	}


	mess.AppendStringWithoutNULL("P2PI");
	mess.AppendUInt16( (uint16_t) type );
	mess.AppendUInt32( (uint32_t) discoveredUsers.size() );


	return true;

}



// only to be used with data messages
int tcp::message_create(int type, CNetworkMessage &mess, std::string theMessage){


	std::string username;

	if( 0x09 != type ){
		return false;
	}

	mess.AppendStringWithoutNULL("P2PI");
	mess.AppendUInt16( (uint16_t) type);
	mess.AppendString(theMessage);


	return true;
}






