#include "Irc.hpp"

Polls::Polls(int fd) : serverFd(fd)
{
    serverPollFds.fd = serverFd;
    serverPollFds.events = POLLIN;
    pollFds.push_back(serverPollFds);
}

void Polls::sendResponse(int client_fd, const std::string& response) {
	std::cout << "Server sent : " << response << std::endl;
	send(client_fd, response.c_str(), response.size(), 0);
}

void Polls::clientDisconnected(int bytes_received, int index) {
	if (bytes_received == 0)
		std::cout << "Client disconnected" << std::endl;
	else
		perror("recv");
	clientsBuffer.erase(pollFds[index].fd); // clear user's buffer
	close(pollFds[index].fd);
	pollFds[index].fd = -1; 
	//? User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.              
	//? Jpense que c'est plus simple que de decaller tous les indexs
}

// void	Polls::modesHandle(int index, const std::string& command)
// {
// 	(void) index;
// 	std::string delim = " ";
// 	std::vector<std::string> split;
// 	for (int j=0; j < split.size(); j++)
// 	{
// 		split.push_back(command.substr(0, command.find(delim)));
// 		std::cout << ">>" << split[0] << std::endl;
// 		command.erase(0, );
// 	}
// 	std::cout << "on y est !!!" << std::endl;
// }

void	Polls::handleClientCommand(int fd, const std::string& command, int index) {
	std::string response;
	std::string prefix = ":server ";
	
	if (command.rfind("CAP", 0) == 0)
		response = "CAP * LS :\r\n"; //! On ignore CAP (notre serveur ne possède aucune capacité de négociation)

	else if (command.rfind("NICK", 0) == 0) { //TODO Il n'y a pas encore de sécurité. A faire.
		response = prefix + "001 ";
		if (tab[index].nickName.empty())
			response += command.substr(5) + " Welcome to the IRC server!\r\n";
		else
			response += "You changed your username to " + command.substr(5);
		tab[index].nickName = command.substr(5);
	}

	else if (command.rfind("USER", 0) == 0) {
		response = prefix + "001 " + tab[index].userName + " Welcome, your user information is received.\r\n";
	}

	else if (command.rfind("MODE", 0) == 0) {

		// modesHandle(index, command);



	} //TODO On ignore MODE pour l'instant

	else if (command.rfind("PING", 0) == 0) {
		response = "PONG :" + command.substr(5) + "\r\n";
	}

	else {
		response = prefix + "421 " + command.substr(0, command.find(' ')) + " :Unknown command\r\n";
	}

	sendResponse(fd, response);
}

void Polls::mainPoll(void)
{	
    while (true) {
        pollCount = poll(pollFds.data(), pollFds.size(), -1);

        if (pollCount == -1) throw StrerrorException("Poll Error");

        for (size_t i = 0; i < pollFds.size(); i++){
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverFd)	//? Si quelqu'un essaie de se connecter
                    createClientPoll();
				else {
					char buffer[1024];
					int bytes_received = recv(pollFds[i].fd, buffer, sizeof(buffer), 0);

					if (bytes_received <= 0)
						clientDisconnected(bytes_received, i);
					else {
						//? Ajouter les données reçues au buffer du client
						clientsBuffer[pollFds[i].fd].append(buffer, bytes_received);

						//? Traiter chaque commande complète (terminée par "\r\n")
						size_t pos;
						while ((pos = clientsBuffer[pollFds[i].fd].find("\r\n")) != std::string::npos) {
							std::string command = clientsBuffer[pollFds[i].fd].substr(0, pos);
							clientsBuffer[pollFds[i].fd].erase(0, pos + 2);

							std::cout << "Received command: " << command << std::endl;
							handleClientCommand(pollFds[i].fd, command, i - 1);
						}
					}
				}
            }
        }
    }
}
//Creer classe ClientPoll en lien avec Polls
void    Polls::createClientPoll(void)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientLen);

    if (clientFd == -1) perror("accept");

    fcntl(clientFd, F_SETFD, O_NONBLOCK);

    clientPollFds.fd = clientFd;
    clientPollFds.events = POLLIN;
    pollFds.push_back(clientPollFds);

    clientsBuffer[clientFd] = "";

	User temp;
    temp.indexInPollFd = pollFds.size() - 1;
	temp.userName = "";
	temp.nickName = "";
	tab.push_back(temp);
    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " (internal id = " << temp.indexInPollFd << ")\n";
}