# include "Server.hpp"

Server::Server(int port) : _port(port)
{
    socketDataSet();
}

void    Server::socketDataSet(void)
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
void    Server::createClient(Polls &poll)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientLen);

    if (clientFd == -1) perror("accept");

    fcntl(clientFd, F_SETFD, O_NONBLOCK);
	poll.addClientPoll(clientFd);

	Client	client(clientFd);
	_clients.push_back(client);
    // clientsBuffer[clientFd] = "";
	
	// User temp; //TODO A decaler dans le constructeur
    // temp.indexInPollFd = _pollFds.size() - 1;
	// temp.userName	= "";
	// temp.id			= "";
	// temp.nickName	= "";
	// temp.host		= "server";
	// temp.fd			= clientFd;
	// temp.registered = false;
	// tab.push_back(temp);
    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " (internal id = " << temp.indexInPollFd << ")\n";
}

// Pas fini: gros travaux !!!
void Server::clientDisconnected(int bytes_received) {
	if (bytes_received == 0)
		std::cout << "Client disconnected" << std::endl;
	else
		perror("recv");
    
	clientsBuffer.erase(_clients[_msg.currentIndex].fd); //? clear user's buffer
	close(_pollFds[_msg.currentIndex].fd);
	//// _pollFds[currentIndex].fd = -1; 
	//// User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.
	//// Jpense que c'est plus simple que de decaller tous les indexs
}