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
		int								i;
	public:
		Polls(void) {};
		Polls(int fd);
		~Polls(void) {};

		int							getI() {return i};
		void			            mainPoll(Server& server);
		void			            addClientPoll(int clientFd);
		void			            erasePoll();
		std::vector<struct pollfd>	getPollFds(void){return(_pollFds);};

};