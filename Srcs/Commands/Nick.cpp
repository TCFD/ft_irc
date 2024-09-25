#include "../../Includes/Server.hpp"

/*
* Erreurs pour la commande NICK :
//? ERR_NONICKNAMEGIVEN (431) : Aucun pseudonyme n'a été fourni.
//? ERR_ERRONEUSNICKNAME (432) : Le pseudonyme fourni contient des caractères non valides.
//? ERR_NICKNAMEINUSE (433) : Le pseudonyme est déjà utilisé par un autre utilisateur.
//! ERR_NICKCOLLISION (436) : Le pseudonyme est en collision avec un pseudonyme déjà utilisé sur un autre serveur.
//! ERR_UNAVAILRESOURCE (437) : Le pseudonyme est temporairement indisponible (par exemple, en raison de restrictions du serveur).
*/

bool is_valid_nick(const std::string& nick) {
	const std::string invalidChars = "@!#$%^&*()+=[]{}\\|:;'\"<>?,/";

	for (std::string::const_iterator it = nick.begin(); it != nick.end(); ++it) {
		if (std::isspace(*it) || invalidChars.find(*it) != std::string::npos) {
			std::cout << "Invalid name, detected invalid char :'" << *it << "'\n";
			return false;
		}
	}
	return true;
}

bool	is_already_exists(const std::string name, const int clientFd, CLIENT_VECC clients)
{

	for (CLIENT_ITC it = clients.begin(); it < clients.end(); it++) {
		if (it->getNickname() == name && it->getFd() != clientFd)
			return true;
	}
	return false;
}

std::string	print_message(std::string num, std::string nickname, std::string message)
{
	if (nickname.empty())
		nickname = "*";
	return (":server " + num + " " + nickname + " " + message + "\r\n");
}

void    print_list_user(CLIENT_VEC&   clients)
{
    std::cout << "List of known users : ";
	for (CLIENT_ITC it = clients.begin(); it < clients.end(); it++) {
		std::cout << it->getNickname() << ",";}
	std::cout << "\n";
}

void	Server::setNick(Client* currentUser, std::string name) {

	currentUser->setOldname(currentUser->getNickname());
	currentUser->setNickname(name);
	if (currentUser->getUsername() != "") {
		currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
		currentUser->setRegistered(true); }
}

//Probleme de NickName a gerer: _ gerer pour un client, mais pas pour +...
void	Server::nick(const int client_fd) {
	const std::string	name			=	_msg.command.substr(5);
	Client		*currentUser			=	&_clients[_msg.currentIndex];
	currentUser->setOldname(currentUser->getNickname());

	for (CHAN_IT it=_channels.begin(); it != _channels.end(); ++it) {
		if (is_user_in_chan(currentUser->getNickname(), *it)) {
			_msg.response = print_message("421", currentUser->getNickname(), " :Can't use this command in a channel");
			return ;}
	}
	if (name.empty()) {
		_msg.response = print_message("431", currentUser->getNickname(), " :No nickname given");}
	else if (!is_valid_nick(name)) {
		_msg.response = print_message("432", "*", name + " :Erroneous nickname"); }
	else if (is_already_exists(name, client_fd, _clients)) {
		_msg.response = print_message("433", "", name + " :Nickname is already in use\r\n");
		}
	else if (!currentUser->getRegistered() && currentUser->getUsername() != "") {
		currentUser->setRegistered(true);
		setNick(currentUser, name); 
		currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
		_msg.response = ":" + currentUser->getNickname() + "!" + currentUser->getUsername() + "@localhost NICK " + currentUser->getNickname() + "\r\n";
		_msg.response += print_message("001", currentUser->getNickname(), ":Welcome to the Internet Relay Network " + currentUser->getId());
	}
	else {
		setNick(currentUser, name); 
		_msg.response = ":" + currentUser->getOldname() + "!" + currentUser->getUsername() + "@localhost NICK " + currentUser->getNickname() + "\r\n";
	}
}
