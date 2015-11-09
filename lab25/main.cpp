#include <list>
#include <iostream>
#include <stdexcept>
#include "Forwarder.h"
#include <arpa/inet.h>


int main(int argc, char **argv) {
	if(argc < 4) {
		std::cerr << "Usage: forwarder <server_port> <host> <host_port>";
		return EXIT_FAILURE;
	}
	int port = std::stoi(argv[1]);
	in_addr host;
	host.s_addr = inet_addr(argv[2]);
	if(0 == host.s_addr) {
		std::cerr << "Bad address";
		return EXIT_FAILURE;
	}
	int host_port = std::stoi(argv[3]);
	try {
		Forwarder *forwarder = new Forwarder((in_port_t) port, host, (in_port_t) host_port);
		forwarder->run();
	} catch (std::runtime_error &e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}