#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "Irc.hpp"

#define SOCKET_CHANNELS	std::map<std::string, std::vector<int> >

class Socket
{
	public:
		Socket(void) {};
		~Socket(void) {};
		SOCKET_CHANNELS saloon;
};

#endif