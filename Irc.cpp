#include "Irc.hpp"

Polls::Polls(int fd) : serverFd(fd)
{
    serverPollFds.fd = serverFd;
    serverPollFds.events = POLLIN;
    pollFds.push_back(serverPollFds);
}

void Polls::send_response(int client_fd, Msg& msg) {
	std::cout << "Server sent " << msg.response << std::endl;
	send(client_fd, msg.response.c_str(), msg.response.size(), 0);
}

void Polls::clientDisconnected(int bytes_received) {
	if (bytes_received == 0)
		std::cout << "Client disconnected" << std::endl;
	else
		perror("recv");
	clientsBuffer.erase(pollFds[msg.currentIndex].fd); //? clear user's buffer
	close(pollFds[msg.currentIndex].fd);
	//// pollFds[currentIndex].fd = -1; 
	//// User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.
	//// Jpense que c'est plus simple que de decaller tous les indexs
}

/* User	Polls::findUser(std::string name) {
	for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++)
		if (name == it->userName)
			return *it;
	return NULL;
} */

void	Polls::handle_client_command(int fd) {
	
	if (msg.command.rfind("CAP", 0) == 0)
		msg.response = "CAP * LS :\r\n"; //! On ignore CAP (notre serveur ne possède aucune capacité de négociation)

	else if (msg.command.rfind("NICK", 0) == 0) { //TODO Il n'y a pas encore de sécurité. A faire.
		nick(msg);
	}

	else if (msg.command.rfind("USER", 0) == 0) {
		msg.response = msg.prefix + "001 " + tab[msg.currentIndex].userName + " Welcome, your user information is received.\r\n";
	}

	else if (msg.command.rfind("MODE", 0) == 0) {

		modesHandle(msg); // faire la reponse du serveur vers le client
	} //TODO On ignore MODE pour l'instant
	else if (msg.command.rfind("JOIN", 0) == 0) {
		channelHandle(msg); std::cout << "Current channel is : " << msg.currentChan << std::endl; }

	else if (msg.command.rfind("PING", 0) == 0) {
		msg.response = msg.prefix + "PONG :" + msg.command.substr(5) + "\r\n"; //? Done.
	}

	else if (msg.command.rfind("QUIT", 0) == 0)
		msg.currentChan = "";
	else if (msg.command.rfind("WHOIS", 0) == 0) {
 		// std::string user = command.substr(6);
		/* User temp = findUser(user);
		if (temp != NULL)

		else */

	}
	else {
		msg.response = msg.prefix + "421 " + msg.command.substr(0, msg.command.find(' ')) + " :Unknown command\r\n";
	}

	send_response(fd, msg);
}

void Polls::mainPoll(void)
{	
    while (true) {
        pollCount = poll(pollFds.data(), pollFds.size(), -1);

        if (pollCount == -1) throw StrerrorException("Poll Error");

		msg.currentChan = "";
		msg.prefix = ":server ";
        for (size_t i = 0; i < pollFds.size(); i++){
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverFd)	//? Si quelqu'un essaie de se connecter
                    createClientPoll();
				else {
					char buffer[1024];
					int bytes_received = recv(pollFds[i].fd, buffer, sizeof(buffer), 0);

					if (bytes_received <= 0)
						clientDisconnected(bytes_received);
					else {
						//? Ajouter les données reçues au buffer du client
						clientsBuffer[pollFds[i].fd].append(buffer, bytes_received);

						//? Traiter chaque commande complète (terminée par "\r\n")
						size_t pos;
						while ((pos = clientsBuffer[pollFds[i].fd].find("\r\n")) != std::string::npos) {
							msg.command = clientsBuffer[pollFds[i].fd].substr(0, pos);
							clientsBuffer[pollFds[i].fd].erase(0, pos + 2);

							std::cout << "Received command: " << msg.command << std::endl;
							msg.currentIndex = i - 1;
							handle_client_command(pollFds[i].fd);
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
	// temp.nickName = ""
	tab.push_back(temp);
    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " (internal id = " << temp.indexInPollFd << ")\n";
}