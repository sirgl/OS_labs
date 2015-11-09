#pragma once
#include <unistd.h>
#include <iostream>


struct Connection {
	static const int BUFFER_SIZE = 4096;
	int client_socket;
	int forwarding_socket;
	int forward_to_client_data_size;
	int client_to_forward_data_size;
	char forward_to_client_buffer[BUFFER_SIZE];
	char client_to_forward_buffer[BUFFER_SIZE];


	Connection(int client_socket, int forwarding_socket) :
			client_socket(client_socket),
			forwarding_socket(forwarding_socket),
			forward_to_client_data_size(0),
			client_to_forward_data_size(0) {
		std::cout << "Connecion created\n";
	}

	virtual ~Connection() {
		close(client_socket);
		close(forwarding_socket);
		std::cout << "Connecion deleted\n";
	}


};
