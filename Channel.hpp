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
		std::string _password;
		int		 _limit;
		int		 _lenClients;
		CLIENT_VEC  _clients;
		CLIENT_VEC  _operators;
		MODES_VEC   _modes;

	public:
		Channel(std::string name);
		~Channel() {};

		std::string	 gName() { return _name; };
		std::string	 gTopic() { return _topic; };
		std::string	 gPassword() { return _password; };
		int			 gLimit() {return _limit; };
		int			 gLenClients() {return _lenClients; };
		MODES_VEC	   gModes() { return _modes; };
		CLIENT_VEC	  gClients() { return _clients; };
		CLIENT_VEC	  gOperators() { return _operators; };
		Client&		 gClientIdx(int idx) { return _clients[idx]; };

		void			sName(std::string name) {_name = name; };
		void			sTopic(std::string topic) {_topic = topic; };
		void			sPwd(std::string pwd) {_password = pwd; };
		void			sLimit(int limit) { _limit = limit; };
		// void			sLenClients(int len) {_lenClients = len; };


		void			addMode(char mod);
		void			addOperator(Client& client);
		void			addClient(Client& client);
		void			addLenClient(void);
		void			dltLenClient(void);
		void			dltMode(char mod);
		void			dltOperator(Client& client);

		bool			isUserOnMe(std::string nick);
};