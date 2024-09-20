#pragma once

# include "Irc.hpp"

class Server;

class Polls
{
	private:
		int								_pollCount;
		struct pollfd					_clientPollFds;
		struct pollfd					_serverPollFds;
		std::vector<struct pollfd>		_pollFds;
		std::map<int, std::string>		_clientsBuffer;

	public:
		Polls(void) {};
		Polls(int fd);
		~Polls(void) {};

		void			            mainPoll(Server& server);
		void			            addClientPoll(int clientFd);
		void			            erasePoll(int i);
		std::vector<struct pollfd>	getPollFds(void){return(_pollFds);};

};