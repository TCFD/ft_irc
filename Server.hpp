#pragma once

# include "Irc.hpp"
# include <map>

class Client;
class Server;

// Define a typedef for a member function pointer
typedef void (Server::*ServerMemberFunction)(Client*);

// Define the map type
typedef std::map<std::string, ServerMemberFunction> DICOCMD;


class Polls
{
	private:
		std::vector<struct pollfd>		_pollFds;
		struct pollfd					_clientPollFds;
		struct pollfd					_serverPollFds;
		int								_pollCount;
		std::map<int, std::string>		_clientsBuffer;

	public:
		Polls() {};
		Polls(int fd);
		~Polls() {};

		// int						 getPollCount() { return(_pollCount); };
		// std::vector<struct pollfd>  getPollsFds(void) {return (_pollFds); };
		// void						setPollCount(int count) {_pollCount = count; };
		void			mainPoll(Server& server);
		void			addClientPoll(int clientFd);
		void			erasePoll(int i);
		std::vector<struct pollfd>	getPollFds(void){return(_pollFds);};
		void			disconnectClient(int i, Server &server);

};

class Server: public Client
{
	private:
		int							_serverSocket;
		int							_limitUsers;
		int							getFdOfUser(std::string nick);
		
		struct sockaddr_in			_serverAddr;

		DICOCMD						_dicocmd;

		bool						isUserOnChannel(std::string nick, std::string targetChannel);
		bool						_quit;
		std::string					_mdp;
		Polls						_poll;
		int							_port;
		std::vector<std::string>	splitCmd(std::string s);
		Msg							_msg;
		CLIENT_VEC					_clients;
		
		CHAN_VEC					_channels;
		CHAN_IT						DoesChanExist (std::string target);

	public:
		~Server(void) {};
		Server(int port, std::string mdp);


	// Getters / Setters
		void			setInChan(bool type);

		int			 	getPort(void)				{return(_port); };
		int			 	getServerSocket(void)		{return(_serverSocket); };

		
		Msg			 	getMsg()					{return(_msg); };
		
		void			setMsgIdx(int idx)			{_msg.currentIndex = idx; };
		void			setMsgCmd(std::string cmd)	{_msg.command = cmd; };
		void			setMsg()					{_msg.currentChan = 0; _msg.prefixServer = ":server "; };
		void			setPoll(Polls& poll)		{_poll = poll; };
		
		DICOCMD			getdicocmd();

		void			socketDataSet(void);

		int			 	createClient(Polls &poll);
   		void			clientDisconnected(int id);
	//-//-//-// TEST
	//-//-//-// FIN TEST

		void			handleClientCommand(int client_fd);
		void			sendResponse(int client_fd);
		STR_VEC			cutModeCommand(void);

	/* * * Command * * */
	// MODE
		int			 	modesHandle(void);

		bool			errorModes(STR_VEC& split);
		bool			errorLenModes(STR_VEC& split);

		void			modes_command(Client *currentUser);
		void			modesOptions(STR_VEC& split);
		void			modeK(STR_VEC& split);
		void			modeO(STR_VEC& split);
		void			modeL(STR_VEC& split);
		void			modeT(STR_VEC& split);
		void			modeI(STR_VEC& split);

	// NICK
		void			nick_command(Client *currentUser);
		void			nick(int client_fd);
		void			setNick(Client* currentUser, std::string name);

	// JOIN
		int			 	join(std::string senderNick);
		
		bool			isChanExists(std::string target);

		void			join_command(Client *currentUser);
		void			sendToChan(std::string message);
		void			sendToEveryone(std::string msg);

	// TOPIC
  		STR_VEC		 	cutTopicCmd(void);

		void			topic_command(Client *currentUser);

		void			topicHandle(Client *currentUser);
		
		bool			errorsTopic(STR_VEC split, Client *currentUser);

	// NAMES
		void			names_command(Client *currentUSer);
		void			namesHandle(void);

	// PRIVMSG 
		void			privmsg_command(Client *currentUser);
		void			privmsg(std::string senderNick);

	// INVITE
		void			invite_command(Client *currentUser);
		void			invite(std::string senderNick);
		void			addClienttoInviteList(std::string name);
	
	// KICK
		void			kick_command(Client *currentUser);
		void			kick(std::string senderNick);

	// USER
		void			user_command(Client *currentUser);

	// CAP
		void			cap_command(Client *currentUser);

	// PING
		void			ping_command(Client *currentUser);

	// QUIT
		void			quit_command(Client *currentUser);

	// WHOIS
		void			whois_command(Client *currentUser);

	// PONG
		void			pong_command(Client *currentUser);

	// PASS
		void			pass_command(Client *currentUser);
		void	    	pass(Client *currenUser);
};

class StrerrorException : public std::exception
{
	private:
		char bufferMessage[256];

	public :
		explicit StrerrorException(const char* message){
			snprintf(bufferMessage, sizeof(bufferMessage), "%s: %s", message, strerror(errno)); }

		virtual const char* what() const throw(){
			return bufferMessage; };
};
