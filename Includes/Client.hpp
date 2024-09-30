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
		const std::string 	getUsername(void) const 		{ return _username; };
		const std::string 	getNickname(void) const 		{ if (!_nickname.empty()) return _nickname; else return ("");};
		const std::string 	getRealname(void) const 		{ return _realname; };
		const std::string 	getOldname(void) const 			{ return _oldname; };
		const std::string 	getHostname(void) const 		{ return _hostname; };
		const std::string 	getId(void) const 				{ return _id; };
		bool				getRegistered(void) const 		{ return _registered; };
		bool				getPasswd(void) const			{ return _passwd; };
		int					getFd(void) const 				{ return _fd; };
		int					getFdC(void) const 				{ return _fd; };
		int					getActualClientFd(void) const	{ return _actualClientFd; };
		
	// Setters
		void			setId(std::string identity) 			{ _id = identity; };
		void			setNickname(std::string name) 			{ _nickname = name; };
		void			setUsername(std::string name) 			{ _username = name; };
		void			setRealname(std::string name) 			{ _realname = name; };
		void			setHostname(std::string name) 			{ _hostname = name; };
		void			setOldname(std::string name) 			{ _oldname = name; };
		void			setRegistered(bool type) 				{ _registered = type; };
		void			setActualClientFd(const int c_fd)		{ _actualClientFd = c_fd; };
		void			setPasswd(bool arg)						{ _passwd = arg; };
		
	// Main Methods
		int		 		parseNickname(std::string name);
};
		