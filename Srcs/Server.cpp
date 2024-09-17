# include "../Server.hpp"

Server::Server(int port, std::string mdp) :  _mdp(mdp), _port(port)
{
	_quit = false;
	socketDataSet();
}

DICOCMD Server::getdicocmd()
{
	DICOCMD dico;

	dico["PRIVMSG"]	= &Server::privmsg_command;
	dico["INVITE"]	= &Server::invite_command;
	dico["MODE"]	= &Server::modes_command;
	dico["NAMES"]	= &Server::names_command;
	dico["TOPIC"]	= &Server::topic_command;
	dico["WHOIS"]	= &Server::whois_command;
	dico["USER"]	= &Server::user_command;
	dico["JOIN"]	= &Server::join_command;
	dico["NICK"]	= &Server::nick_command;
	dico["PING"]	= &Server::ping_command;
	dico["QUIT"]	= &Server::quit_command;
	dico["PONG"]	= &Server::pong_command;
	dico["KICK"]	= &Server::kick_command;
	dico["CAP"]		= &Server::cap_command;
	// dico["PASS"]	= &Server::pass_command;

	return (dico);
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
void Server::clientDisconnected(int id) {
	(void)(id);
	//if (bytes_received == 0)
		std::cout << "Client disconnected" << std::endl;
	//else
	//	perror("recv");
	_clients.erase(_clients.begin() + id); //? clear user's buffer
	//// _pollFds[currentIndex].fd = -1;
	//// User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.
	//// Jpense que c'est plus simple que de decaller tous les indexs
}


void	Server::handleClientCommand(int client_fd)
{
	// Recuperation du dictionnaire de commandes
	DICOCMD	dico = getdicocmd();
	bool founded = false;

	Client	*currentUser = &_clients[_msg.currentIndex];
	_msg.prefixNick = ":" + currentUser->getNickname();

	currentUser->setActualClientFd(client_fd); // Pour nick
	// Verification pour voir si la commande envoyee existe dans le dico
	DICOCMD::iterator it = dico.begin();
	while (it != dico.end())
	{
		// Attention il faudra gerer le cas avec netcat ou il faut enlever le '/' du debut
		if (_msg.command.rfind(it->first, 0) == 0)
		{
			ServerMemberFunction func = it->second;
			(this->*func)(currentUser);
			founded = true;
			break;
		}
		it++ ;
	}

	// Si la commande ne correspond a aucune commande dans le dico
	if (!founded)
		_msg.response = _msg.prefixServer + "421 " + _msg.command.substr(0, _msg.command.find(' ')) + " :Unknown command\r\n";

	sendResponse(client_fd);
	std::cout << "----------------------\n";

}


void Server::sendResponse(int client_fd)
{
	std::cout << "Server sent " << _msg.response << std::endl;
	send(client_fd, _msg.response.c_str(), _msg.response.size(), 0);
	_msg.response.erase();
}

// Separate the command into the all args
STR_VEC Server::splitCmd(std::string s)
{
	std::string delimiter = " ";
	STR_VEC vec;
	
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		vec.push_back(s.substr(0, pos));
		std::cout << "new elmt : " << s.substr(0, pos) << std::endl;
		s.erase(0, pos + delimiter.length());
	}
	std::cout << "new elmt : " << s.substr(0, pos) << std::endl;
	vec.push_back(s);
	return vec;
}
