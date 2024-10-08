#pragma once

# include "Irc.hpp"

class Server;

class Polls
{
	private:
		int								_pollCount;
		struct pollfd					_clientPollFds;
		struct pollfd					_serverPollFds;
		static bool						_quit;
		std::map<int, std::string>		_clientsBuffer;
		std::vector<struct pollfd>		_pollFds;
		std::vector<int>				fdsToDelete;
	public:
		Polls(void) {};
		Polls(int fd) ;
		~Polls(void) {};

		int							isDeleted;
		void			            mainPoll(Server& server);
		void			            addClientPoll(int clientFd);
		void						erasePoll(int i, Server & server);
		void            			disconnectClient(int i, Server &server);
		static void					signalHandler(int sig);
		std::vector<struct pollfd>	getPollFds(void){return(_pollFds);};
		void						pollError(Server &server);
		void						receivedCommandBuffer(Server &server, int i);
		void						isClientHere(Server &server, int i);

};