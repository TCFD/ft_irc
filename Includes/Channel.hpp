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
		int					gLenClients() const  		{ return _lenClients; };
		int			 		gLimit() const  			{ return _limit; };
		std::string			gModesActives(void);
		std::string	 		gName() const 				{ return _name; };
		std::string	 		gTopic() const 				{ return _topic; };
		std::string	 		gTopicName() const 			{ return _topicName; };
		std::string	 		gPassword() const 			{ return _password; };
		const Client&		gClientIdx(int idx) const 	{ return _clients[idx]; };
		CHAR_VEC			gModes() const 				{ return _modes; };
		const CLIENT_VEC&	gClients() const			{ return _clients; };
		const CLIENT_VEC&	gOperators() const			{ return _operators; };
		CLIENT_VEC			gInvite() const 			{ return _invite; };

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