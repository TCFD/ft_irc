#include "../../Includes/Server.hpp"

// ──────────────- ALL COMMANDS -────────────── //

		// - USER COMMAND - //

void	Server::userCommand(Client	*currentUser)
{
	userHandle(currentUser);
}

		// - NAMES COMMAND - //

void	Server::namesCommand(Client *currentUSer)
{
	(void)currentUSer;
	namesHandle();
}

		// - MODES COMMAND - //

void	Server::modesCommand(Client *currentUser)
{
	(void)currentUser;
	modesHandle();
}

		// - JOIN COMMAND - //

void	Server::joinCommand(Client *currentUser)
{
	join(currentUser->getNickname());
}

		// - CAP COMMAND - //

void	Server::capCommand(Client *currentUser)
{
	(void)currentUser;
	_msg.response = "\r\n";
}

		// - NICK COMMAND - //

void	Server::nickCommand(Client *currentUser)
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

void	Server::topicCommand(Client *currentUser)
{
	topicHandle(currentUser);
}


		// - PING COMMAND - //

void	Server::pingCommand(Client *currentUser)
{
	(void)currentUser;
	_msg.response = _msg.prefixServer + "PONG :" + _msg.command.substr(5) + "\r\n"; //? Done.
}


		// - QUIT COMMAND - //

void	Server::quitCommand(Client *currentUser)
{
	(void)currentUser;
	_msg.currentChan = 0;
}

		// - WHOIS COMMAND - //

void	Server::whoisCommand(Client *currentUser)
{
	(void)currentUser;
	return ;
}

		// - PONG COMMAND - //

void	Server::pongCommand(Client *currentUser)
{
	(void)currentUser;
	return ;
}

		// - INVITE COMMAND - //

void	Server::inviteCommand(Client *currentUser)
{
	invite(currentUser->getNickname());
}

		// - PRIVMSG COMMAND - //

void	Server::privMsgCommand(Client *currentUser)
{
	privmsg(currentUser->getNickname());
}

		// - KICK COMMAND - //

void	Server::kickCommand(Client *currentUser)
{
	kick(currentUser->getNickname());
}