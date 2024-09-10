# include "../Server.hpp"

Server::Server(int port) : _port(port)
{
	socketDataSet();
}

void	Server::socketDataSet(void)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw StrerrorException("Socket Error");
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{ throw StrerrorException("Set Socket Options Error"); close (_serverSocket); }

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	memset(&(_serverAddr.sin_zero), '\0', 8);

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
	{ throw StrerrorException("Bind Error"); close(_serverSocket); }

	if (listen(_serverSocket, 10) == -1)
	{ throw StrerrorException("Listen Error"); close(_serverSocket); }

	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
}

//Creer classe ClientPoll en lien avec Polls
int	Server::createClient(Polls &poll)
{
	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientFd = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientLen);

	if (clientFd == -1) perror("accept");

	fcntl(clientFd, F_SETFD, O_NONBLOCK);
	poll.addClientPoll(clientFd);

	Client	client(clientFd);
	_clients.push_back(client);
	
	// std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " (internal id = " << temp.indexInPollFd << ")\n";
	return (clientFd);
}

// Pas fini: gros travaux !!!
void Server::clientDisconnected(int bytes_received, int id) {
	if (bytes_received == 0)
		std::cout << "Client disconnected" << std::endl;
	else
		perror("recv");
	_clients.erase(_clients.begin() + id); //? clear user's buffer
	//// _pollFds[currentIndex].fd = -1;
	//// User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.
	//// Jpense que c'est plus simple que de decaller tous les indexs
}

void	Server::handleClientCommand(int client_fd)
{
	Client	*currentUser = &_clients[_msg.currentIndex];
	_msg.prefixNick = ":" + currentUser->getNickname();

	// _msg.prefixNick = currentUser->getNickname();
	if (_msg.command.rfind("CAP", 0) == 0)
		_msg.response = "\r\n"; //! On ignore CAP (notre serveur ne possède aucune capacité de négociation)

	else if (_msg.command.rfind("NICK", 0) == 0) { /////TODO Il n'y a pas encore de sécurité. A faire.
		nick(client_fd);
	}

	else if (_msg.command.rfind("USER", 0) == 0) {
		currentUser->setUsername(_msg.command.substr(5, _msg.command.find(" ", 5) - 5));
		currentUser->setRealname(_msg.command.substr(_msg.command.find(":")));
		if (currentUser->getNickname() != "") {
			currentUser->setRegistered(true);
			currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
			_msg.response = printMessage("001", currentUser->getNickname(), ":Welcome to the Internet Relay Network " + currentUser->getId());
		}
	}
	else if (_msg.command.rfind("NAMES", 0) == 0) {
		namesHandle(); }

	else if (_msg.command.rfind("MODE", 0) == 0) {
		modesHandle(); // faire la reponse du serveur vers le client
	}
	else if (_msg.command.rfind("JOIN", 0) == 0)
	{	join(currentUser->getNickname()); setInChan(true); }
	
	else if (_msg.command.rfind("TOPIC", 0) == 0) {
		topicHandle(); }

	else if (_msg.command.rfind("PING", 0) == 0) {
		_msg.response = _msg.prefixServer + "PONG :" + _msg.command.substr(5) + "\r\n"; //? Done.
	}

	else if (_msg.command.rfind("QUIT", 0) == 0) {
		_msg.currentChan = 0; setInChan(false); }
	else if (_msg.command.rfind("WHOIS", 0) == 0) {
 		// std::string user = command.substr(6);
		/* User temp = findUser(user);
		if (temp != NULL)

		else */

	}
	else if (_msg.command.rfind("PONG", 0) == 0) {}
	else if (_msg.command.rfind("INVITE", 0) == 0) {
		invite(currentUser->getNickname());
	}
	else if (_msg.command.rfind("PRIVMSG", 0) == 0) {
		privmsg(currentUser->getNickname());
	}
	else if (_msg.command.rfind("KICK", 0) == 0) {
		kick(currentUser->getNickname());
	}
	else {
		_msg.response = _msg.prefixServer + "421 " + _msg.command.substr(0, _msg.command.find(' ')) + " :Unknown command\r\n";
	}
	sendResponse(client_fd);
	// std::cout << RED "IN CHAN VAUT : " << _msg.inChan << NC << std::endl;
}

void Server::sendResponse(int client_fd) {
	std::cout << "Server sent " << _msg.response << std::endl;
	send(client_fd, _msg.response.c_str(), _msg.response.size(), 0);
	_msg.response.erase();
}

// Separate the command into the all args
STR_VEC Server::splitCmd(std::string s) {
	std::string delimiter = " ";
	STR_VEC vec;
	
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		vec.push_back(s.substr(0, pos));
		std::cout << "new elmt : " << s.substr(0, pos) << std::endl;
		s.erase(0, pos + delimiter.length());
	}
	std::cout << "new elmt : " << s.substr(0, pos) << std::endl;
	vec.push_back(s);
	return vec;
}

void	Server::setInChan(bool type) {
	_msg.inChan = type; }