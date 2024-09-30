# include "../Includes/Server.hpp"
#include "../Parsing/includes/Parsing.hpp"

Server::Server(int port, std::string mdp) :  _mdp(mdp), _port(port)
{
	// _quit = false;
	socketDataSet();
}

DICOCMD Server::getDicoCmd()
{
	DICOCMD dico;

	dico["PRIVMSG"]	= &Server::privMsgCommand;
	dico["INVITE"]	= &Server::inviteCommand;
	dico["MODE"]	= &Server::modesCommand;
	dico["NAMES"]	= &Server::namesCommand;
	dico["TOPIC"]	= &Server::topicCommand;
	dico["WHOIS"]	= &Server::whoisCommand;
	dico["USER"]	= &Server::userCommand;
	dico["JOIN"]	= &Server::joinCommand;
	dico["NICK"]	= &Server::nickCommand;
	dico["PING"]	= &Server::pingCommand;
	dico["QUIT"]	= &Server::quitCommand;
	dico["PONG"]	= &Server::pongCommand;
	dico["KICK"]	= &Server::kickCommand;
	dico["CAP"]		= &Server::capCommand;
	dico["PASS"]	= &Server::pass;

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

	// fcntl(clientFd, F_SETFL, O_NONBLOCK); //? May not stay that way. I havent found any issues, but it still does need more testing.
	poll.addClientPoll(clientFd);

	Client	client(clientFd);
	_clients.push_back(client);
	
	return (clientFd);
}

// Pas fini: gros travaux !!!
void Server::clientDisconnected(int id, Client *currentUser) {
	std::cout << "Client disconnected" << std::endl;
	//* Cleaning the channels he was in.
	for (CHAN_IT it = _channels.begin(); it < _channels.end(); it++) {
		if (is_user_in_chan(currentUser->getNickname(), *it)) {
			sendToChan(":" + currentUser->getNickname() + " QUIT :leaving\r\n");
			it->dltClient(currentUser->getNickname());
		}
	}
	std::cout << RED "ID : " << id << NC << std::endl; 
	_clients.erase(_clients.begin() + id);
	// close(currentUser->getFd());
	// _poll.erasePoll(_msg.currentIndex + 1);
	throw std::runtime_error("QUIT WITH NC");
}


bool	Server::isClientTryingToConnect(Client &currentUser, std:: string command) {
	if (command.rfind("NICK", 0) == std::string::npos && command.rfind("USER", 0) == std::string::npos && command.rfind("PASS", 0) == std::string::npos && command.rfind("QUIT", 0) == std::string::npos && !currentUser.getRegistered()) {
		_msg.response = ":server 451 * :You have not registered\r\n";
		return false;
	}
	return true;
}

void	Server::handleClientCommand(const int client_fd)
{
	// Recuperation du dictionnaire de commandes
	DICOCMD	dico = getDicoCmd();

	Client	*currentUser = &_clients[_msg.currentIndex];
	_msg.prefixNick = ":" + currentUser->getNickname();

	std::cout << RED "CLIENT FD: " << client_fd << NC << std::endl;
	currentUser->setActualClientFd(client_fd); // Pour nick
	// Verification pour voir si la commande envoyee existe dans le dico
	//* Si le client n'essaye pas de s'enregistrer alors qu'il ne l'est pas, ciao.
	if (isClientTryingToConnect(*currentUser, _msg.command))
	{
		Parsing	parsingtools;
		try
		{
			std::string concat = getMsg().command;
			std::string cmd = parsingtools.cmdTreatTest(concat);

			DICOCMD::iterator it = dico.find(cmd);

			ServerMemberFunction func = it->second;
			(this->*func)(currentUser);
		}
		catch (Parsing::ParsingInvalidSyntax &e)
		{
			parsingtools.errWriteCorrectForm("");
		}
		catch (Parsing::ParsingInvalidCommand &e)
		{
			_msg.response = _msg.prefixServer + "421 " + _msg.command.substr(0, _msg.command.find(' ')) + " :Unknown command\r\n";
		}
	}
	
	sendResponse(client_fd);
	std::cout << "\n===========================================\n\n";

}

void Server::sendResponse(int client_fd, std::string name)
{

	if (_msg.response == "")
		std::cout << CYAN << "Server didn't send anything.\n" << NC;
	else
		std::cout << CYAN << "Server sent  '" << NC << _msg.response.substr(0, _msg.response.size()-2) << CYAN << "' to "<< BOLD << name << NC << "\n";
	send(client_fd, _msg.response.c_str(), _msg.response.size(), MSG_NOSIGNAL);
	_msg.response.erase();
}

void Server::sendResponse(int client_fd)
{
	if (_msg.response == "")
		std::cout << CYAN << "Server didn't send anything.\n" << NC;
	else
		std::cout << CYAN << "Server sent  '" << NC << _msg.response.substr(0, _msg.response.size()-2) << CYAN << "' to "<< BOLD << _clients[_msg.currentIndex].getNickname() << NC << "\n";
	send(client_fd, _msg.response.c_str(), _msg.response.size(), MSG_NOSIGNAL);
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
		s.erase(0, pos + delimiter.length());
	}
	vec.push_back(s);
	return vec;
}
