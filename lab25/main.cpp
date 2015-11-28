#include <list>
#include <iostream>
#include <stdexcept>
#include "Forwarder.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv) {
	if(argc < 4) {
		std::cerr << "Usage: forwarder <server_port> <host> <forw_port>";
		return EXIT_FAILURE;
	}
	int port = std::stoi(argv[1]);


	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[2], NULL, &hints, &servinfo);
	if (0 != status) {
		std::cerr << "Can't resolve address";
		freeaddrinfo(servinfo);
		return EXIT_FAILURE;
	}
	struct sockaddr_in *host_s = (struct sockaddr_in *) servinfo->ai_addr;



	int host_port = std::stoi(argv[3]);
	try {
		Forwarder *forwarder = new Forwarder((in_port_t) port, host_s->sin_addr, (in_port_t) host_port);
		forwarder->run();
	} catch (std::runtime_error &e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}