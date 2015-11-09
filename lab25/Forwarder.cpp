#include <stdexcept>
#include <unistd.h>
#include "Forwarder.h"
#include "tools.h"


Forwarder::Forwarder(in_port_t listen_port, in_addr forw_addr, in_port_t host_port) :
		server_port(htons(listen_port)),
		host_port(htons(host_port)) {
	server_address.sin_family = AF_INET;
	server_address.sin_port = server_port;
	forwarding_address.sin_addr = forw_addr;
	forwarding_address.sin_family = AF_INET;
	forwarding_address.sin_port = this->host_port;
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_socket) {
		throw std::runtime_error("Failed to create socket: " + get_error(errno));
	}
}


Forwarder::~Forwarder() {
	close(server_socket);
}

void Forwarder::run() {
	int result = bind(server_socket, (const struct sockaddr *) &server_address, sizeof(server_address));
	if (-1 == result) {
		close(server_socket);
		throw std::runtime_error("Failed to bind to port: " + get_error(errno));
	}
	result = listen(server_socket, SOMAXCONN);
	if (-1 == result) {
		close(server_socket);
		throw std::runtime_error("Failed to listen to port: " + get_error(errno));
	}
	int retval;
	socklen_t length;
	while (true) {
		set_select_masks();
		retval = select(FD_SETSIZE, &read_set, &write_set, nullptr, nullptr);
		if (-1 == retval) {
			throw std::runtime_error("Failed to select: " + get_error(errno));
		}
		if (retval == 0) {
			continue;
		}
		if (FD_ISSET(server_socket, &read_set)) {
			int acceptedSocket = accept(server_socket, (sockaddr *) (&server_address), &length);
			if (-1 == acceptedSocket) {
				throw std::runtime_error("Failed to accept client: " + get_error(errno));
			}
			addConnection(acceptedSocket);
		}
		handleConnections();
	}
}

void Forwarder::set_select_masks() {
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_SET(server_socket, &read_set);
	for (auto iterator = connection_list.begin(); iterator != connection_list.end();) {
		if ((*iterator)->forward_to_client_data_size < 0 || (*iterator)->client_to_forward_data_size < 0) {
			delete *iterator;
			connection_list.erase(iterator++);
			continue;
		}
		if ((*iterator)->client_to_forward_data_size == 0) {
			FD_SET((*iterator)->client_socket, &read_set);
		}
		if ((*iterator)->forward_to_client_data_size == 0) {
			FD_SET((*iterator)->forwarding_socket, &read_set);
		}
		if ((*iterator)->forward_to_client_data_size > 0) {
			FD_SET((*iterator)->client_socket, &write_set);
		}
		if ((*iterator)->client_to_forward_data_size > 0) {
			FD_SET((*iterator)->forwarding_socket, &write_set);
		}
		++iterator;
	}
}

void Forwarder::addConnection(int clientSocket) {
	int forwarding_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == forwarding_socket) {
		throw std::runtime_error("Failed to create socket: " + get_error(errno));
	}
	int retval;
	retval = connect(forwarding_socket, (const sockaddr *) (&forwarding_address), sizeof(forwarding_address));
	if (-1 == retval) {
		throw std::runtime_error("Failed to connect: " + get_error(errno));
	}
	Connection *connection = new Connection(clientSocket, forwarding_socket);
	connection_list.push_back(connection);
}

void Forwarder::handleConnections() {
	for (auto connect : connection_list) {
		if (connect->forward_to_client_data_size == 0 && FD_ISSET(connect->forwarding_socket, &read_set)) {
			int res = (int) read(connect->forwarding_socket, connect->forward_to_client_buffer,
			                     Connection::BUFFER_SIZE);
			if (0 == res || -1 == res) {
				connect->forward_to_client_data_size = -1;
			} else {
				connect->forward_to_client_data_size = res;
			}
		}
		if (connect->client_to_forward_data_size == 0 && FD_ISSET(connect->client_socket, &read_set)) {
			int res = (int) read(connect->client_socket, connect->client_to_forward_buffer, Connection::BUFFER_SIZE);
			if (0 == res || -1 == res) {
				connect->client_to_forward_data_size = -1;
			} else {
				connect->client_to_forward_data_size = res;
			}
		}
		if (connect->forward_to_client_data_size > 0 && FD_ISSET(connect->client_socket, &write_set)) {
			int res = (int) write(connect->client_socket, connect->forward_to_client_buffer,
			                      (size_t) connect->forward_to_client_data_size);
			if (-1 == res || res == 0) {
				connect->forward_to_client_data_size = -1;
			} else {
				connect->forward_to_client_data_size = 0;
			}
		}
		if (connect->client_to_forward_data_size > 0 && FD_ISSET(connect->forwarding_socket, &write_set)) {
			int res = (int) write(connect->forwarding_socket, connect->client_to_forward_buffer,
			                      (size_t) connect->client_to_forward_data_size);
			if (res == -1 || res == 0) {
				connect->client_to_forward_data_size = -1;
			} else {
				connect->client_to_forward_data_size = 0;
			}
		}
	}
}
