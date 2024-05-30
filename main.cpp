#include "Irc.hpp"

Socket test;

char *substr(char *str, int start, int end)
{
	char *buf;
	buf = (char *)malloc(end - start + 1);
	int j = 0;
	if (start > end)
		return NULL;
	while (str[start] && start < end) {
		buf[j] = str[start]; start++; j++;
	}
	buf[j] = 0;
	std::cout << buf << std::endl;
	return buf;
}

int create_server_socket(int port) {
	int server_fd;
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE); }

	// Définir les options du socket
	// int opt = 1;
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
	// 	perror("setsockopt");
	// 	close(server_fd);
	// 	exit(EXIT_FAILURE);
	// }

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	memset(&(server_addr.sin_zero), '\0', 8);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE); }
	if (listen(server_fd, 10) == -1) {
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE); }
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	return server_fd;
}

void send_response(int client_fd, const std::string& response) {
	send(client_fd, response.c_str(), response.size(), 0);
}

int main(int ac, char **av) {

	if (ac != 3)
		return(1);
	int port = atoi(av[1]);
	int server_fd = create_server_socket(port);
	std::vector<struct pollfd> poll_fds;

	struct pollfd server_poll_fd;
	server_poll_fd.fd = server_fd;
	server_poll_fd.events = POLLIN;
	poll_fds.push_back(server_poll_fd);

	std::map<int, std::string> client_buffers;

	std::cout << "Server is running on port " << port << std::endl;

	while (true) {
		int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);

		if (poll_count == -1) {
			perror("poll");
			break;
		}

		for (size_t i = 0; i < poll_fds.size(); ++i) {
			if (poll_fds[i].revents & POLLIN) {
				if (poll_fds[i].fd == server_fd) {
					struct sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);
					int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

					if (client_fd == -1) {
						perror("accept");
						continue; }

					fcntl(client_fd, F_SETFL, O_NONBLOCK);

					struct pollfd client_poll_fd;
					client_poll_fd.fd = client_fd;
					client_poll_fd.events = POLLIN;
					poll_fds.push_back(client_poll_fd);

					client_buffers[client_fd] = "";

					std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
				} else {
					char buffer[1024];
					int bytes_received = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);

					if (bytes_received <= 0) {
						if (bytes_received == 0) {
							std::cout << "Client disconnected" << std::endl; }
						else { perror("recv"); }

						close(poll_fds[i].fd);
						client_buffers.erase(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
					} else {
						// Ajouter les données reçues au buffer du client
						client_buffers[poll_fds[i].fd].append(buffer, bytes_received);

						// Traiter chaque commande complète (terminée par "\r\n")
						size_t pos;
						while ((pos = client_buffers[poll_fds[i].fd].find("\r\n")) != std::string::npos) {
							std::string command = client_buffers[poll_fds[i].fd].substr(0, pos);
							client_buffers[poll_fds[i].fd].erase(0, pos + 2);

							std::cout << "Received command: " << command << std::endl;
							handle_client_command(poll_fds[i].fd, command, test);
						}
					}
				}
			}
		}
	}
	close(server_fd);
	return 0;
}
