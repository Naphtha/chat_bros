#include "p2pim_udp.h"

// for debugging purposes
#include <bitset>

// create a udp message of any type
int udp::message_create(int type, std::string *params, CNetworkMessage &mess){

    uint16_t udp_port;
    uint16_t tcp_port;

    std::string hostname;
    std::string username;


    udp_port = atoi( params[1].c_str() );
    tcp_port = atoi( params[2].c_str() );

    hostname = params[5];
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
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);
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
























