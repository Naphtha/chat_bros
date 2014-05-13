#include "p2pim_tcp.h"
#include "p2pim_udp.h"
#include <termios.h>


// avoid namespace issues
using std::cout;
using std::endl;

#define BUFFER_SIZE 256
#define MAX_CLIENTS 32

// defaults for program
const char *DEFAULT_UDP = "50550";
const char *DEFAULT_TCP = "50551";
const char *DEFAULT_MIN_TIMEOUT = "5";
const char *DEFAULT_MAX_TIMEOUT = "60"; 

// struct for keeping track of userlist
struct user_info{
	int TCP_port;
	std::string username;
	std::string hostname;
};

// forward declarations
void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts, int *num_hosts);
void signal_handler(int param);
long parse_ip(std::string the_ip_string);
void set_noncanonical_mode(int fd, struct termios *savedattributes);
void reset_noncanonical_mode(int fd, struct termios *savedattributes);
void add_discovered_user( char *packet_buffer , user_info *user_list, int num_users);



int main(int argc, char **argv){

	// for non_canonical_mode
    struct termios saved_term_attr;
    char rx_char;

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
	// 3 is min number of sockets required for operation
	// 0 is udp, 1 is tcp, 2 is STDINN
	pollfd file_descriptors[MAX_CLIENTS + 3];
	nfds_t num_fds = 0;
	int udp_socket_fd;
	int tcp_socket_fd;
	int poll_return = 0;

	// server address
	sockaddr_in udp_server_address;
	sockaddr_in tcp_server_address;
	// client address
	sockaddr_in udp_client_address;
	sockaddr_in tcp_client_address;
	// address lengths
	socklen_t udp_client_length;
	socklen_t tcp_client_length;

	// connected clients
	user_info connected_clients[MAX_CLIENTS];
	int num_users;
	
	// check for message sendto success
	int send_check;

	int timeout_val;
	// temp_string used for swapping strings around
	std::string temp_string;


	if( (argc % 2) != 1){
		// if flags and arguments dont come in pairs
		// e.g. -u bill vs. -u
		cout << "Sorry, it seems an argument was incorrectly formatted." << endl;
		exit(0);
	}

	parse_arguments(argc, argv, arguments, external_hosts, &num_extern_hosts);

	timeout_val = atoi( arguments[3].c_str() );

	bzero(file_descriptors, sizeof(file_descriptors));

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


	// initialize all sockets and add them to the fd array
	udp::initialize(arguments, &udp_server_address, &udp_socket_fd, file_descriptors);
	num_fds++;
	tcp::initialize(arguments, &tcp_server_address, &tcp_socket_fd, file_descriptors);
	num_fds++;
	// initialize non canonical mode and add to fd array
	set_noncanonical_mode(STDIN_FILENO, &saved_term_attr);
	file_descriptors[2].fd = STDIN_FILENO;
	file_descriptors[2].events = POLLIN;
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


	// broadcast
	if( udp::message_create(1, arguments, udp_packet_buffer, &udp_buffer_size) ){
		cout << "Message create failed." << endl;
		exit(EXIT_SUCCESS);
	}
	send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
		   0, (sockaddr *)&udp_server_address, sizeof(udp_server_address));
	if(send_check < 0){
		perror("Error in message send.");
		exit(0);
	}

	// cout << "Broadcasted." << endl;



	while(1){

		// if timeout
		if( (poll_return = poll(file_descriptors, num_fds, timeout_val * 1000) ) == 0){

			cout << "Poll timed out with value " << timeout_val << endl;
			// set new timeout val
			timeout_val *= 2;
			if( timeout_val  > atoi(arguments[4].c_str()) ){
				timeout_val = atoi(arguments[4].c_str());
			}

			// broadcast
			if( udp::message_create(1, arguments, udp_packet_buffer, &udp_buffer_size) ){
				cout << "Message create failed." << endl;
				exit(EXIT_SUCCESS);
			}
			send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
				   0, (sockaddr *)&udp_server_address, sizeof(udp_server_address));
			if(send_check < 0){
				perror("Error in message send.");
				exit(0);
			}
			// cout << "Broadcasted." << endl;

			
		}
		else{
			
			// cout << "The poll return value was: " << poll_return << endl;

			// first file descriptor is always udp socket
			// if we receive something on the udp socket
			if(file_descriptors[0].revents == POLLIN){


				udp_client_length = sizeof(udp_client_address);

				recvfrom(file_descriptors[0].fd, udp_packet_buffer, BUFFER_SIZE, 0,
						 (sockaddr *)&udp_client_address, &udp_client_length );

				// if packet came from us, ignore it
				if( !strcmp(&(udp_packet_buffer[10]), arguments[5].c_str()) ){
					cout << "Received self broadcast." << endl;
					continue;
				}

				// if packet is discovery, reply
				if( udp_packet_buffer[5] == 0x01){
					// craft a discovery message using 2 as arg
					cout << "Receieved broadcast from " << &(udp_packet_buffer[10]) << endl;



					udp::message_create(2, arguments, udp_packet_buffer, &udp_buffer_size);
					send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
						   0, (sockaddr *)&udp_server_address, sizeof(udp_server_address));
					if(send_check < 0){
						perror("Error in message send.");
						exit(0);
					}
				}

				// if packet is reply
				if( udp_packet_buffer[5] == 0x02){

					cout << "Receieved reply message from " << &(udp_packet_buffer[10]) << endl;
				}

				// if packet is close
				if( udp_packet_buffer[5] == 0x03 ){

					cout << "Receieved closing message from " << &(udp_packet_buffer[10]) << endl;
				}
			} // end of udp revents


			// if tcp file descriptor recieves stuff
			if(file_descriptors[1].revents == POLLIN){

				cout << "Receieved a TCP message." << endl;
				break;




			} // end of tcp revents


			// if STDIN recieves a key
			if(file_descriptors[2].revents == POLLIN){
				read(STDIN_FILENO, &rx_char, 1);

				// e caught, exit
				if( 'e' == rx_char){
					break;
				}
				// l caught, list clients
				if( 'l' == rx_char){
					cout << "List of clients: " << endl;
				}


			}







			for( int i = 0; i < 32; i++ ){
				if(file_descriptors[i].revents == POLLERR){
					cout << "Caught a POLLERR on " << i << endl;
				}
				if(file_descriptors[i].revents == POLLHUP){
					cout << "Caught a POLLHUP on " << i << endl;
				}
			}
		}// end poll return with POLLIN block







	}

	reset_noncanonical_mode(STDIN_FILENO, &saved_term_attr);

	// broadcast closing message
	udp::message_create(3, arguments, udp_packet_buffer, &udp_buffer_size);
	send_check = sendto(udp_socket_fd, udp_packet_buffer, udp_buffer_size,
		   0, (sockaddr *)&udp_server_address, sizeof(udp_server_address));
	if(send_check < 0){
		perror("Error in message send.");
		exit(0);
	}

	// close socket file descriptors
	for(unsigned int i = 0; i < num_fds; i++ ){
		cout << "Closing FD # " << file_descriptors[i].fd << endl;
		close(file_descriptors[i].fd);

	}

	return 0;
}



void add_discovered_user( char *packet_buffer , user_info *user_list, int num_users){

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

    for( unsigned int i = 0; i < strlen(name); i++ ){
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


// thanks for these functions Prof. Nitta
void set_noncanonical_mode(int fd, struct termios *savedattributes){
    struct termios TermAttributes;
    
    // Make sure stdin is a terminal. 
    if(!isatty(fd)){
        fprintf (stderr, "Not a terminal.\n");
        exit(0);
    }
    
    // Save the terminal attributes so we can restore them later. 
    tcgetattr(fd, savedattributes);
    
    // Set the funny terminal modes. 
    tcgetattr (fd, &TermAttributes);
    TermAttributes.c_lflag &= ~(ICANON | ECHO); // Clear ICANON and ECHO. 
    TermAttributes.c_cc[VMIN] = 1;
    TermAttributes.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSAFLUSH, &TermAttributes);
}

void reset_noncanonical_mode(int fd, struct termios *savedattributes){
    tcsetattr(fd, TCSANOW, savedattributes);
}










