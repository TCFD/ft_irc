#include "../Includes/Server.hpp"
#include "../Parsing/includes/Parsing.hpp"

Polls::Polls(int fd)
{
	// _quit = false;
    _serverPollFds.fd = fd;
    _serverPollFds.events = POLLIN;
	_serverPollFds.revents = 0;
    _pollFds.push_back(_serverPollFds);
}

void	Polls::signalHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "CTRL C detected: quit the server\n";
		_quit = true;
	}
}

void	Polls::erasePoll(int i)
{
	_pollFds.erase(_pollFds.begin() + i);
	// close (_pollFds[i].fd);
}

void Polls::mainPoll(Server& server)
{
    while (true)
	{
        _pollCount = poll(_pollFds.data(), _pollFds.size(), -1);

        if (_pollCount == -1 || _quit == true)
		{
			// disconnectClient(_pollFds.size() -1, server);
			close(server.getServerSocket());
			for (std::vector<int>::iterator it = fdsToDelete.begin(); it != fdsToDelete.end(); it++)
				close(*it);
			throw StrerrorException("Poll Error");
		}

		server.setMsg();	
        for (size_t i = 0; i < _pollFds.size(); i++)
		{
            if (_pollFds[i].revents & POLLIN)
			{
                if (_pollFds[i].fd == server.getServerSocket())	//? Si quelqu'un essaie de se connecter
                {
					server.createClient(*this);
					std::cout << "NEW INCOMING CONNECTION" << std::endl;
				}
				else
				{
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int bytes_received = recv(_pollFds[i].fd, buffer, sizeof(buffer), 0);
          
					if (bytes_received <= 0)
						erasePoll(i);
					else
					{

					//? Ajouter les données reçues au buffer du client
						_clientsBuffer[_pollFds[i].fd].append(buffer, bytes_received);
						memset(buffer, 0, sizeof(buffer));

					//? Traiter chaque commande complète (terminée par "\r\n")
					
						size_t pos;
						while ((pos = _clientsBuffer[_pollFds[i].fd].find("\n")) != std::string::npos)
						{
							if (_clientsBuffer[_pollFds[i].fd].find("\r") != std::string::npos)
								pos--;
							server.setMsgCmd(_clientsBuffer[_pollFds[i].fd].substr(0, pos));
							_clientsBuffer[_pollFds[i].fd].erase(0, pos + 2); 
							Parsing	parsingtools;

							try
							{
								std::string concat = server.getMsg().command;
								if (concat == "/HELP")
									parsingtools.parsingHelp();
								parsingtools.cmdTreatTest(concat);
							}
							catch (std::exception &e)
							{
//								std::cout << e.what() << std::endl;
								// parsingtools.cmdStatus();
								parsingtools.errWriteCorrectForm("");
								continue;
							}
							server.setMsgIdx(i - 1);
							std::string nickOfCurrentUser = server.gNickClient();
							if (nickOfCurrentUser == "")
								nickOfCurrentUser = "new user";
							if (_clientsBuffer[_pollFds[i].fd] != "")
								std::cout << BLUE << nickOfCurrentUser << "'s command buffer: " << "'" << NC << _clientsBuffer[_pollFds[i].fd] << BLUE << "'" << NC << std::endl;
							else
								std::cout << BLUE << nickOfCurrentUser << "'s command buffer is empty\n" << NC;
							std::cout << GREEN <<  "Received command: '" << NC << server.getMsg().command << GREEN << "'" << NC << std::endl;
							try {
								server.handleClientCommand(_pollFds[i].fd);
							}
							catch (std::runtime_error &e) {
								erasePoll(i);
							}
						}
					}
				}
            }
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