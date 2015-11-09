#pragma once

#include <netinet/in.h>
#include <list>
#include "Connection.h"

class Forwarder {

	in_port_t host_port;
	in_port_t server_port;
	int server_socket;
	struct sockaddr_in server_address;
	struct sockaddr_in forwarding_address;
	fd_set read_set;
	fd_set write_set;
	std::list<Connection*> connection_list;

public:

	Forwarder(in_port_t listen_port, in_addr forw_addr, in_port_t host_port);

	virtual ~Forwarder();

	void run();

private:
	void set_select_masks();

	void addConnection(int clientSocket);

	void handleConnections();
};
