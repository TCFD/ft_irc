#pragma once

# include "Irc.hpp"

class Client
{
	protected:
		std::string _username;
		std::string _nickname;
		std::string _realname;
		std::string _oldname;
		std::string _hostname;
		std::string _id;
		bool		_registered;
		int		 _fd;

	public:
		Client() {};
		Client(int clientFd);
		~Client(void) {};

		std::string getUsername(void) {return _username; };
		std::string getNickname(void) {return _nickname; };
		std::string getRealname(void) {return _realname; };
		std::string getOldname(void) {return _oldname; };
		std::string getHostname(void) { return _hostname; };
		std::string getId(void) { return _id; };
		bool		getRegistered(void) {return _registered; };
		int		 getFd(void) { return _fd; };

		void		setId(std::string identity) { _id = identity; };
		void		setNickname(std::string name) { _nickname = name; };
		void		setUsername(std::string name) { _username = name; };
		void		setRealname(std::string name) { _realname = name; };
		void		setHostname(std::string name) { _hostname = name; };
		void		setOldname(std::string name) { _oldname = name; };
		void		setRegistered(bool type) { _registered = type; };
		
		int		 parseNickname(std::string);
};
		