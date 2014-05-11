#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

// avoid namespace issues
using std::cout;
using std::endl;

// defaults for program
const unsigned short DEFAULT_UDP = 50550;
const unsigned short DEFAULT_TCP = 50551;
const unsigned short DEFAULT_MIN_TIMEOUT = 5;
const unsigned short DEFAULT_MAX_TIMEOUT = 60; 

// forward declarations
void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts);





int main(int argc, char **argv){

	std::string username;
	std::string arguments[6];
	std::string external_hosts[8];

	if( (argc % 2) != 1){
		// if flags and arguments dont come in pairs
		// e.g. -u bill vs. -u
		cout << "Sorry, it seems an argument was incorrectly formatted." << endl;
		exit(0);
	}



	parse_arguments(argc, argv, arguments, external_hosts);


	cout << arguments[0] << endl;
	cout << arguments[1] << endl;
	cout << arguments[2] << endl;
	cout << arguments[3] << endl;
	cout << arguments[4] << endl;
	cout << arguments[5] << endl;




	cout << "Hello World" << endl;
	

	username = getenv("USER");


	return 0;
}


void parse_arguments(int argc, char **argv, std::string *arguments, std::string *external_hosts){

	// set default arguments and then overwrite them

	if( getenv("USER") )
		arguments[0] = getenv("USER");
	else{
		cout << "Problem getting Username from environment.";
		exit(0);
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
			// arguments with -pp 
			
		}
		else{
			cout << "There was a problem parsing the arguments." << endl;
		}


	}





	return;
};


















