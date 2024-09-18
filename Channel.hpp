#pragma once

#include "Irc.hpp"

class Client;

class Channel {

	private:
		std::string 	_name;
		std::string 	_topic;
		std::string 	_topicName;
		std::string 	_password;
		int				_limit;
		int				_lenClients;
		CHAR_VEC    	_modes;
		CLIENT_VEC  	_clients;
		CLIENT_VEC  	_operators;
		CLIENT_VEC		_invite;

	public:
		Channel(std::string name);
		~Channel(void) {};

	// Getters
		int				gLenClients() 		{ return _lenClients; };
		int			 	gLimit() 			{ return _limit; };
		std::string		gModesActives(void);
		std::string	 	gName() 			{ return _name; };
		std::string	 	gTopic() 			{ return _topic; };
		std::string	 	gTopicName() 		{ return _topicName; };
		std::string	 	gPassword() 		{ return _password; };
		Client&			gClientIdx(int idx) { return _clients[idx]; };
		CHAR_VEC		gModes() 			{ return _modes; };
		CLIENT_VEC		gClients() 			{ return _clients; };
		CLIENT_VEC		gOperators() 		{ return _operators; };
		CLIENT_VEC		gInvite() 			{ return _invite; };

	// Setters
		void			sName(std::string name) {_name = name; };
		void			sTopic(std::string topic) {_topic = topic; };
		void			sTopicName(std::string nick) {_topicName = nick; };
		void			sPwd(std::string pwd) {_password = pwd; };
		void			sLimit(int limit) { _limit = limit; };

	// Add
		void			addMode(char mod);
		void			addOperator(Client& client);
		void			addOperatorByName(std::string name, CLIENT_VEC cli);
		void			addClient(Client& client);
		void			addLenClient(void);
		void			addInvite(Client& client);
	
	// Delete
		void			dltLenClient(void);
		void			dltMode(char mod);
		void			dltOperator(std::string name);
		void			dltClient(std::string name);
		void			dltInvite(std::string name);

	// Utils
		bool			isUserOnMe(std::string nick);
		bool			isUserInvite(std::string name);
};	