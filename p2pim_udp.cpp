#include "p2pim_udp.h"
// for debugging purposes
#include <bitset>

// returns 0 on success

int udp::message_create(short type, const std::string *params, char *buffer){

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

	// successful return
	return 0;
}



int initialize(pollfd *file_descriptors, std::string *args){



	return 0;
}


























