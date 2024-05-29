#include <iostream>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <map>

#define SOCKET_CHANNELS	std::map<std::string, std::vector<int>>

class Socket
{
	public:
		Socket(void) {};
		~Socket(void) {};
		SOCKET_CHANNELS saloon;
};

Socket test;
int create_server_socket(int port) {
	int server_fd;
	struct sockaddr_in server_addr;

	// Création du socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Définir les options du socket
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		perror("setsockopt");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// Configurer l'adresse du serveur
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	memset(&(server_addr.sin_zero), '\0', 8);

	// Lier le socket à l'adresse et au port
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// Écouter les connexions entrantes
	if (listen(server_fd, 10) == -1) {
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// Mettre le socket en mode non-bloquant
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	return server_fd;
}

void send_response(int client_fd, const std::string& response) {
	send(client_fd, response.c_str(), response.size(), 0);
}

void handle_client_command(int client_fd, const std::string& command) {
	std::string response;
	std::string prefix = ":localhost ";

	if (command.rfind("NICK", 0) == 0) {
		response = prefix + "001 Welcome to the IRC server!\r\n";
	} else if (command.rfind("USER", 0) == 0) {
		response = prefix + "001 Welcome, your user information is received.\r\n";
	} else if (command.rfind("PING", 0) == 0) {
		response = "PONG :" + command.substr(5) + "\r\n";
	} else if (command.rfind("CAP", 0) == 0) {
		response = "CAP * LS :\r\n"; // Simplement ignorer CAP pour cette implémentation
	} else if (command.rfind("JOIN", 0) == 0) {
		response = prefix + "JOIN " + command.substr(5) + "\r\n";
		test.saloon[command.substr(5)].push_back(client_fd);
		std::cout << command.substr(5) << " now has new client registered\n"; 
	}
	else if (command.rfind("PRIVMSG", 0) == 0) {
		std::string message = command.substr(command.find(':'));
		std::string	channel = command.substr(8, command.find(':') - 9);
		std::cout << "user wants to send " << message << " to " << channel<< std::endl;

		//:alice!~alice@host PRIVMSG #discussion :Bonjour tout le monde!

		std::string end_msg = ":username!~username@host PRIVMSG " + channel + " :" + message + "\r\n";
		for (std::vector<int>::iterator it = test.saloon[channel].begin(); it != test.saloon[channel].end(); ++it) {
			if (*it != client_fd) {
				send_response(*it, end_msg);
			}
		}	

	}
	else {
		response = prefix + "421 " + command.substr(0, command.find(' ')) + " :Unknown command\r\n";
	}

	send_response(client_fd, response);
}

int main() {
	int port = 6667;
	int server_fd = create_server_socket(port);
	std::vector<struct pollfd> poll_fds;

	// Ajouter le descripteur de fichier du serveur à la liste de poll
	struct pollfd server_poll_fd;
	server_poll_fd.fd = server_fd;
	server_poll_fd.events = POLLIN; // Surveiller les événements de type POLLIN
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
			if (poll_fds[i].revents & POLLIN) { // Vérifier les événements de type POLLIN
				if (poll_fds[i].fd == server_fd) { // Si c'est le descripteur de fichier du serveur
					// Nouvelle connexion entrante
					struct sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);
					int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

					if (client_fd == -1) {
						perror("accept");
						continue;
					}

					fcntl(client_fd, F_SETFL, O_NONBLOCK); // Mettre le nouveau socket en mode non-bloquant

					struct pollfd client_poll_fd;
					client_poll_fd.fd = client_fd;
					client_poll_fd.events = POLLIN; // Surveiller les événements de type POLLIN
					poll_fds.push_back(client_poll_fd);

					client_buffers[client_fd] = ""; // Initialiser le buffer pour le nouveau client

					std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
				} else {
					// Données reçues d'un client existant
					char buffer[1024];
					int bytes_received = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);

					if (bytes_received <= 0) {
						// Déconnexion du client
						if (bytes_received == 0) {
							std::cout << "Client disconnected" << std::endl;
						} else {
							perror("recv");
						}

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
							handle_client_command(poll_fds[i].fd, command);
						}
					}
				}
			}
		}
	}

	close(server_fd);
	return 0;
}
