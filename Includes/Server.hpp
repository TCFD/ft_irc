#pragma once

# include "Irc.hpp"

class Client;

// Define a typedef for a member function pointer
typedef void (Server::*ServerMemberFunction)(Client*);

// Define the map type
typedef std::map<std::string, ServerMemberFunction> DICOCMD;

class Server {

	private:
		struct sockaddr_in			_serverAddr;
		std::string					_mdp;
		CLIENT_VEC					_clients;
		CHAN_VEC					_channels;
		DICOCMD						_dicocmd;
		Polls						_poll;
		bool						_quit;
		Msg							_msg;
		int							_serverSocket;
		int							_limitUsers;
		int							_port;

	public:
		Server(int port, std::string mdp);
		~Server(void) {};

	// Getters
		int				getFdOfUser(std::string nick);
		int			 	getPort(void)					{return(_port); };
		int			 	getServerSocket(void)			{return(_serverSocket); };
		Msg			 	getMsg(void)					{return(_msg); };
		DICOCMD			getDicoCmd(void);
		
	// Setters
		void			setMsgResponse(std::string response)	{_msg.response = response;};
		void			setMsgIdx(int idx)						{_msg.currentIndex = idx; };
		void			setMsgCmd(std::string cmd)				{_msg.command = cmd; };
		void			setMsg(void)							{_msg.currentChan = 0; _msg.prefixServer = ":server "; };
		void			setPoll(Polls& poll)					{_poll = poll; };
		
	// Main Methods
		int			 				createClient(Polls &poll);
		void						socketDataSet(void);
   		void						clientDisconnected(int id);
		void						handleClientCommand(int client_fd);
		void 						sendResponse(int client_fd);
		void						sendResponse(int client_fd, std::string name);
		bool						isUserOnChannel(std::string nick, std::string targetChannel);
		STR_VEC						cutModeCommand(void);
		CHAN_ITC					DoesChanExist (std::string target);
		std::vector<std::string>	splitCmd(std::string s);

	/* * * Command * * */
	// MODE
		int			 	modesHandle(void);
		void			modesCommand(Client *currentUser);
		void			modesOptions(STR_VEC& split);
		void			modeK(STR_VEC& split);
		void			modeO(STR_VEC& split);
		void			modeL(STR_VEC& split);
		void			modeT(STR_VEC& split);
		void			modeI(STR_VEC& split);
		bool			errorLenModes(STR_VEC& split);
		bool			errorModes(STR_VEC& split);

	// NICK
		void			nickCommand(Client *currentUser);
		void			nick(int client_fd);
		void			setNick(Client* currentUser, std::string name);

	// JOIN
		int			 	join(std::string senderNick);
		void			joinCommand(Client *currentUser);
		void			sendToChan(std::string message);
		void			sendToEveryone(std::string msg);
		bool			isChanExists(std::string target);

	// TOPIC
  		STR_VEC		 	cutTopicCmd(void);
		void			topicCommand(Client *currentUser);
		void			topicHandle(Client *currentUser);
		bool			errorsTopic(STR_VEC split, Client *currentUser);

	// NAMES
		void			namesHandle(void);
		void			namesCommand(Client *currentUSer);

	// PRIVMSG
		void			privmsg(std::string senderNick);
		void			privMsgCommand(Client *currentUser);
		void			sendPrivmsgToUser(std::string senderNick, std::string destinataire, std::string msg);
		void			sendPrimsgToChannel(std::string senderNick, std::string destinataire, std::string msg);

	// INVITE
		void			invite(std::string senderNick);
		void			inviteCommand(Client *currentUser);
		void			addClienttoInviteList(std::string name);
	
	// KICK
		void			kick(std::string senderNick);
		void			kickCommand(Client *currentUser);

	// USER
		void    		userHandle(Client   *currentUser);
		void			userCommand(Client *currentUser);

	// CAP
		void			capCommand(Client *currentUser);

	// PING
		void			pingCommand(Client *currentUser);

	// QUIT
		void			quitCommand(Client *currentUser);

	// WHOIS
		void			whoisCommand(Client *currentUser);

	// PONG
		void			pongCommand(Client *currentUser);

	// PASS
		void	    	pass(Client *currenUser);

		std::string			getNickOfCurrentClient(void) {return _clients[_msg.currentIndex].getNickname();};
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
