#include "../Includes/Server.hpp"

Polls::Polls(int fd)
{
    _serverPollFds.fd = fd;
    _serverPollFds.events = POLLIN;
	_serverPollFds.revents = 0;
    _pollFds.push_back(_serverPollFds);
	isDeleted = -1;
}

void	Polls::signalHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "CTRL C detected: quit the server\n";
		_quit = true;
	}
}

void	Polls::erasePoll(int i, Server & server)
{
	std::cout << RED << "Ereasing someone from pollFds.\n" << NC;
	std::cout << "isdeleted is equal to " << isDeleted << std::endl;
	if (isDeleted == -1) {
		try {server.clientDisconnected(i + 1, server.getClient());}
		catch (std::runtime_error &e) {}
	}
	isDeleted = -1;
	_pollFds.erase(_pollFds.begin() + i);
}

// Check if the client's poll is valid or if the server is closed by ctrlC
void	Polls::pollError(Server &server)
{
	_pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
	if (_pollCount == -1 || _quit == true)
	{
		close(server.getServerSocket());
		for (std::vector<int>::iterator it = fdsToDelete.begin(); it != fdsToDelete.end(); it++)
			close(*it);
		throw StrerrorException("Poll Error");
	}
}

// Process the command from the client
void	Polls::receivedCommandBuffer(Server &server, int i)
{
	size_t pos;
	while ((pos = _clientsBuffer[_pollFds[i].fd].find("\n")) != std::string::npos)
	{
		if (_clientsBuffer[_pollFds[i].fd].find("\r") != std::string::npos)
			pos--;
		server.setMsgCmd(_clientsBuffer[_pollFds[i].fd].substr(0, pos));
		_clientsBuffer[_pollFds[i].fd].erase(0, pos + 2);
		server.setMsgIdx(i - 1);
		std::string nickOfCurrentUser = server.gNickClient();
		if (nickOfCurrentUser == "")
			nickOfCurrentUser = "new user";
		if (_clientsBuffer[_pollFds[i].fd] != "")
			std::cout << BLUE << nickOfCurrentUser << "'s command buffer: " << "'" << NC << _clientsBuffer[_pollFds[i].fd] << BLUE << "'" << NC << std::endl;
		else
			std::cout << BLUE << nickOfCurrentUser << "'s command buffer is empty\n" << NC;
		std::cout << GREEN <<  "Received command: '" << NC << server.getMsg().command << GREEN << "'" << NC << std::endl;
		try 
		{	server.handleClientCommand(_pollFds[i].fd);	}
		catch (std::runtime_error &e) 
		{	erasePoll(i, server);	}
	}
}

// Connect a new client or process the command from an existed client
void	Polls::isClientHere(Server &server, int i)
{
	if (_pollFds[i].fd == server.getServerSocket()) {
		server.createClient(*this);
		std::cout << "NEW INCOMING CONNECTION" << std::endl; }
	else {
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		int bytes_received = recv(_pollFds[i].fd, buffer, sizeof(buffer), 0);
		if (bytes_received <= 0)
			erasePoll(i, server);
		else {
			_clientsBuffer[_pollFds[i].fd].append(buffer, bytes_received);
			memset(buffer, 0, sizeof(buffer));
				receivedCommandBuffer(server, i); } }
}

void Polls::mainPoll(Server& server)
{
    while (true)
	{
        pollError(server);
		server.setMsg();	
        for (size_t i = 0; i < _pollFds.size(); i++)
		{
            if (_pollFds[i].revents & POLLIN) {
                isClientHere(server, i); }
        }
    }
	close(server.getServerSocket());
}

void	Polls::addClientPoll(int clientFd)
{
	_clientPollFds.fd = clientFd;
    _clientPollFds.events = POLLIN;
    _pollFds.push_back(_clientPollFds);
	fdsToDelete.push_back(clientFd);
}