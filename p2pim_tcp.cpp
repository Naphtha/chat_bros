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



int tcp::messsage_create(short type, std::string *params, char *buffer, int *buffer_size){

	int buffer_index = 6;

	bzero( buffer, sizeof(buffer) );

	buffer[0] = 'P';
	buffer[1] = '2';
	buffer[2] = 'P';
	buffer[3] = 'I';
	buffer[4] = 0x00;
	buffer[5] = type;


	if(0x04 == type){
		for( int i = 0; i < params[0].length(); i++ ){
			buffer[buffer_index++] = params[0][i];
		}

		buffer[buffer_index++] = '\0';

		*buffer_size = buffer_index - 1;
	}
	else{

		*buffer_size = buffer_index;
	}

	return 0;

}












