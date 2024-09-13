#include "../Server.hpp"
#include "../Parsing/includes/Parsing.hpp"

Polls::Polls(int fd)
{
    _serverPollFds.fd = fd;
    _serverPollFds.events = POLLIN;
    _pollFds.push_back(_serverPollFds);
}

// void Polls::send_response(int client_fd) {
// 	std::cout << "Server sent " << _msg.response << std::endl;
// 	send(client_fd, _msg.response.c_str(), _msg.response.size(), 0);
// 	_msg.response.erase();
// }

// void Polls::clientDisconnected(int bytes_received) {
// 	if (bytes_received == 0)
// 		std::cout << "Client disconnected" << std::endl;
// 	else
// 		perror("recv");
// 	_clientsBuffer.erase(_pollFds[_msg.currentIndex].fd); //? clear user's buffer
// 	close(_pollFds[_msg.currentIndex].fd);
// 	//// _pollFds[currentIndex].fd = -1; 
// 	//// User on index x isn't connected anymore. For future reference, when fd = -1, ignore user.
// 	//// Jpense que c'est plus simple que de decaller tous les indexs
// }

// void	Polls::handle_client_command(int client_fd) {
// 	User	*currentUser = &tab[_msg.currentIndex];

// 	_msg.prefixNick = currentUser->nickName;
// 	if (_msg.command.rfind("CAP", 0) == 0)
// 		_msg.response = "\r\n"; //! On ignore CAP (notre serveur ne possède aucune capacité de négociation)

// 	else if (_msg.command.rfind("NICK", 0) == 0) { /////TODO Il n'y a pas encore de sécurité. A faire.
// 		nick(client_fd);
// 	}

// 	else if (_msg.command.rfind("USER", 0) == 0) {
// 		currentUser->userName = _msg.command.substr(5, _msg.command.find(" ", 5) - 5);
// 		currentUser->realName = _msg.command.substr(_msg.command.find(":"));
// 		if (currentUser->nickName != "") {
// 			// if (currentUser->nickName.find("_") != std::string::npos && currentUser->userName.find("_") == std::string::npos)
// 			// 	{currentUser->userName += "_";}
// 			currentUser->registered = true;
// 			currentUser->id = currentUser->nickName + "!" + currentUser->userName + "@" + currentUser->host;
// 			_msg.response = printMessage("001", currentUser->nickName, ":Welcome to the Internet Relay Network " + currentUser->id);
// 		}
// 	}

// 	else if (_msg.command.rfind("MODE", 0) == 0) {
// 		modesHandle(); // faire la reponse du serveur vers le client
// 	}
// 	else if (_msg.command.rfind("JOIN", 0) == 0) {
// 		join(); }

// 	else if (_msg.command.rfind("PING", 0) == 0) {
// 		_msg.response = _msg.prefixServer + "PONG :" + _msg.command.substr(5) + "\r\n"; //? Done.
// 	}

// 	else if (_msg.command.rfind("QUIT", 0) == 0) {
// 		_msg.currentChan = 0; }
// 	else if (_msg.command.rfind("WHOIS", 0) == 0) {
//  		// std::string user = command.substr(6);
// 		/* User temp = findUser(user);
// 		if (temp != NULL)

// 		else */

// 	}
// 	else if (_msg.command.rfind("PONG", 0) == 0) {}
// 	else {
// 		_msg.response = _msg.prefixServer + "421 " + _msg.command.substr(0, _msg.command.find(' ')) + " :Unknown command\r\n";
// 	}

// 	send_response(client_fd);
// 	_msg.response.clear();
// }

void Polls::mainPoll(Server& server)
{	
    while (true)
	{
        _pollCount = poll(_pollFds.data(), _pollFds.size(), -1);

        if (_pollCount == -1) throw StrerrorException("Poll Error");

		server.setMsg();
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
						std::cout << "Deleting elmt " << server.getMsg().currentIndex << "\n";
						server.clientDisconnected(bytes_received, server.getMsg().currentIndex); 
						_pollFds.erase(_pollFds.begin() + i);
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
									parsingtools.parsing_help();
								parsingtools.cmd_treat_test(concat);
							}
							catch (std::exception &e)
							{
								std::cout << e.what() << std::endl;
								parsingtools.err_write_correct_form("");
							}
							std::cout << "command client buffer: " << _clientsBuffer[_pollFds[i].fd] << std::endl;
							std::cout << "Received command: " << server.getMsg().command << std::endl;
							server.setMsgIdx(i - 1);
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