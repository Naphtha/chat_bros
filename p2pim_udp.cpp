#include "p2pim_udp.h"

// for debugging purposes
#include <bitset>

// returns 0 on success

int udp::message_create(short type, std::string *params, char *buffer, int *buffer_size){

	// initialize default packet structure

	unsigned short udp_port;
	unsigned short tcp_port;
	unsigned short buffer_index = 10;

	char Buffer[256];
    struct hostent *LocalHostEntry;
    char name[256];


	bzero( buffer, sizeof(buffer) );

	buffer[0] = 'P';
	buffer[1] = '2';
	buffer[2] = 'P';
	buffer[3] = 'I';
	buffer[4] = 0x00;
	buffer[5] = type;

	udp_port = atoi( params[1].c_str() );
	buffer[6] = (udp_port >> 8) ;
	buffer[7] = udp_port;

	tcp_port = atoi( params[2].c_str() );
	buffer[8] = (tcp_port >> 8);
	buffer[9] = tcp_port;



    if(-1 == gethostname(Buffer, 255)){
        return -1;
    }
    LocalHostEntry = gethostbyname(Buffer);
    if(NULL == LocalHostEntry){
        return -1;
    }
    strcpy(name, LocalHostEntry->h_name);



	for( int i = 0; i < strlen(name); i++ ){
		buffer[buffer_index++] = name[i];
	}

	buffer[buffer_index++] = '\0';


	for( int i = 0; i < params[0].length(); i++ ){
		buffer[buffer_index++] = params[0][i];
	}

	buffer[buffer_index++] = '\0';

	*buffer_size = buffer_index - 1;

	// successful return
	return 0;
}


int udp::message_create(int type, std::string *params, CNetworkMessage &mess){

    uint16_t udp_port;
    uint16_t tcp_port;

    char Buffer[256];
    struct hostent *LocalHostEntry;
    std::string hostname;
    std::string username;


    udp_port = atoi( params[1].c_str() );
    tcp_port = atoi( params[2].c_str() );

    if(-1 == gethostname(Buffer, 255)){
        return false;
    }
    LocalHostEntry = gethostbyname(Buffer);
    if(NULL == LocalHostEntry){
        return false;
    }
    hostname = LocalHostEntry->h_name;

    username = params[0];

    mess.Clear();

    mess.AppendStringWithoutNULL("P2PI");
    mess.AppendUInt16((uint16_t) type);
    mess.AppendUInt16(udp_port);
    mess.AppendUInt16(tcp_port);
    mess.AppendString(hostname);
    mess.AppendString(username);




    return true;
}


void udp::initialize(std::string *args, sockaddr_in *server_address, int *socket_file_descriptor,
					 pollfd *file_descriptors){

	int result;
	int broadcast_enable;
	int reuse_enable;
	int port_number;
	int port_reuse_enable;

    *socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(0 > *socket_file_descriptor){
        perror("Error opening socket");
    }


    // Set UDP socket options
    // broadcast
    // reuseaddr
    // reuseport
    broadcast_enable = 1;
    result = setsockopt(*socket_file_descriptor, SOL_SOCKET, SO_BROADCAST,
    					&broadcast_enable, sizeof(broadcast_enable));

    reuse_enable = 1;
    result += setsockopt(*socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR,
    					&reuse_enable, sizeof(reuse_enable));

    port_reuse_enable = 1;
    result += setsockopt(*socket_file_descriptor, SOL_SOCKET, SO_REUSEPORT,
    					&port_reuse_enable, sizeof(port_reuse_enable));
    if(0 > result){
        close(*socket_file_descriptor);
        perror("Error setting socket option.");
    }

    // get port from args
    port_number = atoi( args[1].c_str() );

    // clean out server address
    bzero((char *) server_address, sizeof(*server_address));
    // set address values
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = htonl(INADDR_BROADCAST);
    server_address->sin_port = htons(port_number);

    // bind sockets
    if(0 > bind(*socket_file_descriptor, (struct sockaddr *)server_address, sizeof(*server_address))){ 
        perror("Error binding socket: ");
    }

    // add to filedescriptor array
    file_descriptors[0].fd = *socket_file_descriptor;
    file_descriptors[0].events = POLLIN;



    return;
}
























