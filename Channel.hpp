#pragma once

#include "Client.hpp"
#include "Irc.hpp"
#include <iostream>
#include <vector>

class Client;

# define CLIENT_VEC		 std::vector<Client>
# define MODES_VEC		  std::vector<char>
# define CLIENT_IT		  CLIENT_VEC::iterator
# define CHAN_IT			CHAN_VEC::iterator

class Channel {

	private:
		std::string _name;
		std::string _topic;
		std::string _topicName;
		// std::string	_topicDate;
		std::string _password;
		int			_limit;
		int			_lenClients;
		CLIENT_VEC  _clients;
		CLIENT_VEC  _operators;
		MODES_VEC   _modes;

	public:
		Channel(std::string name);
		~Channel() {};

		std::string	 	gName() { return _name; };
		std::string	 	gTopic() { return _topic; };
		std::string	 	gTopicName() { return _topicName; };
		// std::string		gTopicDate() { return _topicDate; };
		std::string	 	gPassword() { return _password; };
		int			 	gLimit() {return _limit; };
		int				gLenClients() {return _lenClients; };
		MODES_VEC		gModes() { return _modes; };
		std::string		gModesActives(void);
		CLIENT_VEC		gClients() { return _clients; };
		CLIENT_VEC		gOperators() { return _operators; };
		Client&			gClientIdx(int idx) { return _clients[idx]; };
		// std::string		getTime(void);

		void			sName(std::string name) {_name = name; };
		void			sTopic(std::string topic) {_topic = topic; };
		void			sTopicName(std::string nick) {_topicName = nick; };
		void			sPwd(std::string pwd) {_password = pwd; };
		void			sLimit(int limit) { _limit = limit; };
		// void			sLenClients(int len) {_lenClients = len; };

		void			addMode(char mod);
		void			addOperator(Client& client);
		void			addOperatorByName(std::string name, CLIENT_VEC cli);
		void			addClient(Client& client);
		void			addLenClient(void);
		void			dltLenClient(void);
		void			dltMode(char mod);
		void			dltOperator(std::string name);

		bool			isUserOnMe(std::string nick);
};	