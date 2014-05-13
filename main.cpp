#include "p2pim_tcp.h"
#include "p2pim_udp.h"


// avoid namespace issues
using std::cout;
using std::endl;

#define BUFFER_SIZE 256

// defaults for program
const char *DEFAULT_UDP = "50550";
const char *DEFAULT_TCP = "50551";
const char *DEFAULT_MIN_TIMEOUT = "5";
const char *DEFAULT_MAX_TIMEOUT = "60"; 

// forward declarations
void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts, int *num_hosts);

void signal_handler(int param);

long parse_ip(std::string the_ip_string);



int main(int argc, char **argv){

	// arrays of strings for arguments
	std::string arguments[6];
	std::string external_hosts[8];
	sockaddr_in external_addresses[8];
	int num_extern_hosts = 0;

	// packet buffers and lengths
	char udp_packet_buffer[BUFFER_SIZE];
	int udp_buffer_size;
	char tcp_packet_buffer[BUFFER_SIZE];
	int tcp_buffer_size;

	// set of connections for poll
	pollfd file_descriptors[32];
	nfds_t num_fds = 0;
	int udp_socket_fd;
	int tcp_socket_fd;


	// server address
	sockaddr_in udp_server_address;
	sockaddr_in tcp_server_address;
	// client address
	sockaddr_in udp_client_address;
	sockaddr_in tcp_client_address;
	// address lengths
	socklen_t udp_client_length;
	socklen_t tcp_client_length;
	
	// gotten a chat partner
	bool got_bro = false;
	// check for message sendto success
	int send_check;

	int timeout_val;
	std::string temp_string;


	if( (argc % 2) != 1){
		// if flags and arguments dont come in pairs
		// e.g. -u bill vs. -u
		cout << "Sorry, it seems an argument was incorrectly formatted." << endl;
		exit(0);
	}

	parse_arguments(argc, argv, arguments, external_hosts, &num_extern_hosts);

	timeout_val = atoi( arguments[3].c_str() );

	signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGUSR1, signal_handler);


	cout << arguments[0] << endl;
	cout << arguments[1] << endl;
	cout << arguments[2] << endl;
	cout << arguments[3] << endl;
	cout << arguments[4] << endl;
	cout << arguments[5] << endl;

	cout << external_hosts[0] << endl;
	cout << external_hosts[1] << endl;

	cout << num_extern_hosts << endl;


	udp::initialize(arguments, &udp_server_address, &udp_socket_fd, file_descriptors);
	num_fds++;
	tcp::initialize(arguments, &tcp_server_address, &tcp_socket_fd, file_descriptors);
	num_fds++;



	// parse args and place into external_hosts array
	for( int i = 0; i < num_extern_hosts; i++){
		// set ipv4
		external_addresses[i].sin_family = AF_INET;
		// slice out ip address
		temp_string = external_hosts[i].substr( 0, external_hosts[i].find(":") );
		// parse ip and place into s_addr
		external_addresses[i].sin_addr.s_addr = htonl( parse_ip(temp_string) );
		// slice out port number
		temp_string = external_hosts[i].substr( external_hosts[i].find(":") + 1, external_hosts[i].length() );
		// place port number into sin_port
		external_addresses[i].sin_port = htons(atoi(temp_string.c_str()));
	}

	// code to send to each external address
	for(int i = 0; i < num_extern_hosts; i++){
		// allocate udp_packet
		if( udp::message_create(1, arguments, udp_packet_buffer, &udp_buffer_size) ){
			cout << "Message create failed." << endl;
			exit(EXIT_SUCCESS);
		}
		// send udp packet 
		send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
			   0, (sockaddr *)&(external_addresses[i]), sizeof(external_addresses[i]));
		if(send_check < 0){
			cout << "Error in message send." << endl;
			exit(0);
		}
		cout << "sent a message to external host" << endl;
	}




	while(1){

		// if no chat partners found
		if( !got_bro ){
			// broadcast
			if( udp::message_create(1, arguments, udp_packet_buffer, &udp_buffer_size) ){
				cout << "Message create failed." << endl;
				exit(EXIT_SUCCESS);
			}
			send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
				   0, (sockaddr *)&udp_server_address, sizeof(udp_server_address));
			if(send_check < 0){
				cout << "Error in message send." << endl;
				exit(0);
			}

			cout << "Broadcasted." << endl;
		}

		if( poll(file_descriptors, num_fds, timeout_val * 1000) == 0){

			if( (timeout_val * 2) <= atoi(arguments[4].c_str()) ){
				timeout_val *= 2;
				continue;
			}
			else{
				timeout_val = atoi(arguments[4].c_str());
				continue;
			}


		}
		else{
			
			// first file descriptor is always udp socket
			// if we receive something on the udp socket
			if(file_descriptors[0].revents == POLLIN){

				udp_client_length = sizeof(udp_client_address);

				recvfrom(file_descriptors[0].fd, udp_packet_buffer, BUFFER_SIZE, 0,
						 (sockaddr *)&udp_client_address, &udp_client_length );

				// if packet came from us, ignore it

				// cout << &(udp_packet_buffer[10]) << endl;



			}





		for( int i = 0; i < num_fds; i++ ){
			if(file_descriptors[i].revents == POLLIN){
				cout << "Caught a POLLIN Event at fd # " << file_descriptors[i].fd << endl;
				file_descriptors[i].revents = 0;
			}
		}



		}







	}




	for(unsigned int i = 0; i < num_fds; i++ ){
		cout << "Closing FD # " << file_descriptors[i].fd << endl;
		close(file_descriptors[i].fd);

	}

	return 0;
}

void signal_handler(int param){
	cout << "Caught signal " << param << endl;
    exit(EXIT_SUCCESS);
}


void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts, int *num_hosts){

	unsigned char hosts_index = 0;
	char Buffer[BUFFER_SIZE];
	char name[BUFFER_SIZE];
    struct hostent *LocalHostEntry;

	// set default arguments and then overwrite them

	if( getenv("USER") )
		arguments[0] = getenv("USER");
	else{
		cout << "Problem getting Username from environment.";
		exit(0);
	}

	arguments[1] = DEFAULT_UDP;
	arguments[2] = DEFAULT_TCP;
	arguments[3] = DEFAULT_MIN_TIMEOUT;
	arguments[4] = DEFAULT_MAX_TIMEOUT;

    if(-1 == gethostname(Buffer, 255)){
    	perror("Hostname get failed.");
    	exit(0);
    }
    LocalHostEntry = gethostbyname(Buffer);
    if(NULL == LocalHostEntry){
    	perror("Hostname get failed.");
    	exit(0);
    }
    strcpy(name, LocalHostEntry->h_name);

    for( int i = 0; i < strlen(name); i++ ){
    	arguments[5].push_back(name[i]);
    }


	for( int i = 1; i < argc; i+=2 ){

		// if find -u flag
		if( !strcmp(argv[i], "-u") ){
			// first argument is username
			arguments[0] = argv[i+1];
		}
		else if( !strcmp(argv[i], "-up") ){
			// second argument is udp port
			arguments[1] = argv[i+1];
		}
		else if( !strcmp(argv[i], "-tp" ) ){
			// third is tcp port
			arguments[2] = argv[i+1];
		}
		else if( !strcmp(argv[i], "-dt") ){
			// fourth is initial timeout
			arguments[3] = argv[i+1];
		}
		else if ( !strcmp(argv[i], "-dm") ){
			// fifth is max timeout
			arguments[4] = argv[i+1];
		}
		else if( !strcmp(argv[i], "-pp") ){
			// arguments with -pp stored in external_hosts
			external_hosts[hosts_index] = argv[i+1];
			//increment index
			hosts_index++;
			// keeps track of the extra broadcast messages to send
			(*num_hosts)++;
		}
		else{
			cout << "There was a problem parsing the arguments." << endl;
		}
	}
	return;
};


long parse_ip(std::string the_ip_string){

	std::string delim = ".";
	size_t pos;
	long ip_bits = 0;

	while ((pos = the_ip_string.find(delim)) != std::string::npos) {
	    ip_bits += atoi( (the_ip_string.substr(0, pos)).c_str() );
	    the_ip_string.erase(0, pos + delim.length());
	    ip_bits <<= 8;
	}

	ip_bits += atoi( (the_ip_string.substr(0, pos)).c_str() );
    // std::cout << std::hex << ip_bits << std::endl;

	return ip_bits;
}















