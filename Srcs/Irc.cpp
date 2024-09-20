#include "../Includes/Server.hpp"
#include "../Parsing/includes/Parsing.hpp"

Polls::Polls(int fd)
{
    _serverPollFds.fd = fd;
    _serverPollFds.events = POLLIN;
    _pollFds.push_back(_serverPollFds);
}

void	Polls::erasePoll(int i) {
	_pollFds.erase(_pollFds.begin() + i);
}

void Polls::disconnectClient(int i, Server & server) {
	server.clientDisconnected(server.getMsg().currentIndex);
	// _poll.erasePoll(i);
	_pollFds.erase(_pollFds.begin() + i);
}


void Polls::mainPoll(Server& server)
{
    while (true)
	{
        _pollCount = poll(_pollFds.data(), _pollFds.size(), -1);

        if (_pollCount == -1) throw StrerrorException("Poll Error");

		server.setMsg();
		//server._quit = 0;
        for (size_t i = 0; i < _pollFds.size(); i++)
		{
            if (_pollFds[i].revents & POLLIN) 
			{
				// std::cout << RED "CLIENT FD IS " << _pollFds[i].fd << NC << std::endl;
                if (_pollFds[i].fd == server.getServerSocket())	//? Si quelqu'un essaie de se connecter
                {
					server.createClient(*this);
					std::cout << "NEW INCOMING CONNECTION : " << _pollFds[i].fd << std::endl;
				}
						// _clientsBuffer[idx] = "";}
				else
				{
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int bytes_received = recv(_pollFds[i].fd, buffer, sizeof(buffer), 0);
          
					if (bytes_received <= 0)
					{
						server.clientDisconnected(server.getMsg().currentIndex); 
						erasePoll(i);
						//close(_pollFds[server.getMsg().currentIndex].fd); 
					}
					else
					{
						//std::cout << "buffer = " << buffer << std::endl;

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
								std::cout << e.what() << std::endl;
								parsingtools.cmdStatus();
								parsingtools.errWriteCorrectForm("");
								continue;
							}
							server.setMsgIdx(i - 1);
							std::string nickOfCurrentUser = server.getNickOfCurrentClient();
							if (nickOfCurrentUser == "")
								nickOfCurrentUser = "new user";
							if (_clientsBuffer[_pollFds[i].fd] != "")
								std::cout << BLUE << nickOfCurrentUser << "'s command buffer: " << "'" << NC << _clientsBuffer[_pollFds[i].fd] << BLUE << "'" << NC << std::endl;
							else
								std::cout << BLUE << nickOfCurrentUser << "'s command buffer is empty\n" << NC;
							std::cout << GREEN <<  "Received command: '" << NC << server.getMsg().command << GREEN << "'" << NC << std::endl;
							server.handleClientCommand(_pollFds[i].fd);
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
}