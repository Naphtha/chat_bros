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
void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts);

void signal_handler(int param);




int main(int argc, char **argv){

	// set of connections for poll
	pollfd file_descriptors[16];
	// arrays of strings for arguments
	std::string arguments[6];
	std::string external_hosts[8];
	// gotten a chat partner
	bool got_bro = false;

	char udp_packet_buffer[BUFFER_SIZE];



	if( (argc % 2) != 1){
		// if flags and arguments dont come in pairs
		// e.g. -u bill vs. -u
		cout << "Sorry, it seems an argument was incorrectly formatted." << endl;
		exit(0);
	}

	parse_arguments(argc, argv, arguments, external_hosts);

	signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGUSR1, signal_handler);


	// cout << arguments[0] << endl;
	// cout << arguments[1] << endl;
	// cout << arguments[2] << endl;
	// cout << arguments[3] << endl;
	// cout << arguments[4] << endl;
	// cout << arguments[5] << endl;

	// cout << external_hosts[0] << endl;
	// cout << external_hosts[1] << endl;
	// cout << external_hosts[2] << endl;
	// cout << external_hosts[3] << endl;

	udp::initialize(file_descriptors, arguments);


	while(1){

		// if no chat partners found
		if( !got_bro ){
			// broadcast
			if( udp::message_create(1, arguments, udp_packet_buffer) ){
				cout << "Message create failed." << endl;
				exit(EXIT_SUCCESS);
			}

		}








	}






	return 0;
}

void signal_handler(int param){
	cout << "Caught signal " << param << endl;
    exit(EXIT_SUCCESS);
}


void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts){

	unsigned char hosts_index = 0;

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
		}
		else{
			cout << "There was a problem parsing the arguments." << endl;
		}
	}
	return;
};


















