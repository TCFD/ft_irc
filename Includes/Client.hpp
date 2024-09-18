#pragma once

# include "Irc.hpp"

class Client
{
	protected:
		std::string 	_username;
		std::string 	_nickname;
		std::string 	_realname;
		std::string 	_oldname;
		std::string 	_hostname;
		std::string 	_id;
		bool			_registered;
		bool			_passwd;
		int				_actualClientFd;
		int		 		_fd;

	public:
		Client(void) {};
		Client(int clientFd);
		~Client(void) {};

	// Getters
		std::string 	getUsername(void) 		{ return _username; };
		std::string 	getNickname(void) 		{ return _nickname; };
		std::string 	getRealname(void) 		{ return _realname; };
		std::string 	getOldname(void) 		{ return _oldname; };
		std::string 	getHostname(void) 		{ return _hostname; };
		std::string 	getId(void) 			{ return _id; };
		bool			getRegistered(void) 	{ return _registered; };
		bool			getPasswd(void)			{ return _passwd; };
		int				getFd(void) 			{ return _fd; };
		int				getActualClientFd(void)	{ return _actualClientFd; };
		
	// Setters
		void			setId(std::string identity) 	{ _id = identity; };
		void			setNickname(std::string name) 	{ _nickname = name; };
		void			setUsername(std::string name) 	{ _username = name; };
		void			setRealname(std::string name) 	{ _realname = name; };
		void			setHostname(std::string name) 	{ _hostname = name; };
		void			setOldname(std::string name) 	{ _oldname = name; };
		void			setRegistered(bool type) 		{ _registered = type; };
		void			setActualClientFd(int c_fd)		{ _actualClientFd = c_fd; };
		void			setPasswd(bool arg)				{ _passwd = arg; };
		
	// Main Methods
		int		 		parseNickname(std::string name);
};
		