#include "../../Server.hpp"


// ──────────────- ALL COMMANDS -────────────── //

		// - USER COMMAND - //

void	Server::user_command(Client *currentUser)
{
	if (currentUser->getPasswd()) {
		currentUser->setUsername(_msg.command.substr(5, _msg.command.find(" ", 5) - 5));
		currentUser->setRealname(_msg.command.substr(_msg.command.find(":")));
		if (currentUser->getNickname() != "") {
			currentUser->setRegistered(true);
			currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
			_msg.response = printMessage("001", currentUser->getNickname(), ":Welcome to the Internet Relay Network " + currentUser->getId());
		}
	}
	else
	{
		_msg.response = ":server Please set your password first\r\n";
		sendResponse(currentUser->getFd());
		_msg.response += ":server KILL * :Invalid password, please retry.\r\n";
		sendResponse(currentUser->getFd());
	}
}

		// - NAMES COMMAND - //

void	Server::names_command(Client *currentUSer)
{
	(void)currentUSer;
	namesHandle();
}

		// - MODES COMMAND - //

void	Server::modes_command(Client *currentUser)
{
	(void)currentUser;
	modesHandle();
}

		// - JOIN COMMAND - //

void	Server::join_command(Client *currentUser)
{
	join(currentUser->getNickname());
}

		// - CAP COMMAND - //

void	Server::cap_command(Client *currentUser)
{
	(void)currentUser;
	_msg.response = "\r\n";
}

		// - NICK COMMAND - //

void	Server::nick_command(Client *currentUser)
{
	if (currentUser->getPasswd()) {
		nick(currentUser->getActualClientFd());
	}
	else
	{
		_msg.response = ":server Please set your password first\r\n";
		sendResponse(currentUser->getFd());
		_msg.response += ":server KILL * :Invalid password, please retry.\r\n";
		sendResponse(currentUser->getFd());
	}
}


		// - TOPIC COMMAND - //

void	Server::topic_command(Client *currentUser)
{
	topicHandle(currentUser);
}


		// - PING COMMAND - //

void	Server::ping_command(Client *currentUser)
{
	(void)currentUser;
	_msg.response = _msg.prefixServer + "PONG :" + _msg.command.substr(5) + "\r\n"; //? Done.
}


		// - QUIT COMMAND - //

void	Server::quit_command(Client *currentUser)
{
	(void)currentUser;
	_msg.currentChan = 0;
}

		// - WHOIS COMMAND - //

void	Server::whois_command(Client *currentUser)
{
	(void)currentUser;
	return ;
}

		// - PONG COMMAND - //

void	Server::pong_command(Client *currentUser)
{
	(void)currentUser;
	return ;
}

		// - INVITE COMMAND - //

void	Server::invite_command(Client *currentUser)
{
	invite(currentUser->getNickname());
}

		// - PRIVMSG COMMAND - //

void	Server::privmsg_command(Client *currentUser)
{
	privmsg(currentUser->getNickname());
}

		// - KICK COMMAND - //

void	Server::kick_command(Client *currentUser)
{
	kick(currentUser->getNickname());
}