#include "Server.hpp"

/*
* Erreurs pour la commande NICK :
//? ERR_NONICKNAMEGIVEN (431) : Aucun pseudonyme n'a été fourni.
//? ERR_ERRONEUSNICKNAME (432) : Le pseudonyme fourni contient des caractères non valides.
//? ERR_NICKNAMEINUSE (433) : Le pseudonyme est déjà utilisé par un autre utilisateur.
//! ERR_NICKCOLLISION (436) : Le pseudonyme est en collision avec un pseudonyme déjà utilisé sur un autre serveur.
//! ERR_UNAVAILRESOURCE (437) : Le pseudonyme est temporairement indisponible (par exemple, en raison de restrictions du serveur).
*/

bool isValidNick(const std::string& nick) {
	const std::string invalidChars = "@!#$%^&*()+=[]{}\\|:;'\"<>?,/";

	for (std::string::const_iterator it = nick.begin(); it != nick.end(); ++it) {
		if (std::isspace(*it) || invalidChars.find(*it) != std::string::npos) {
			std::cout << "Invalid name, detected invalid char :'" << *it << "'\n";
			return false;
		}
	}
	return true;
}

bool	isAlreadyExists(std::string name, int clientFd, CLIENT_VEC clients)
{
	for (CLIENT_IT it = clients.begin(); it < clients.end(); it++) {
		if (it->getNickname() == name && it->getFd() != clientFd)
			return true;
	}
	return false;
}

std::string	printMessage(std::string num, std::string nickname, std::string message)
{
	if (nickname.empty())
		nickname = "*";
	return (":localhost " + num + " " + nickname + " " + message + "\r\n");
}

void    printListUser(CLIENT_VEC&   clients)
{
    std::cout << "List of known users : ";
	for (CLIENT_IT it = clients.begin(); it < clients.end(); it++) {
		std::cout << it->getNickname() << ",";}
	std::cout << "\n";
}

void	Server::setNick(Client* currentUser, std::string name) {

	currentUser->setOldname(currentUser->getNickname());
	currentUser->setNickname(name);
	if (currentUser->getUsername() != "") {
		currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
		currentUser->setRegistered(true); }
	// std::cout << MAGENTA "NAME ==> " << currentUser->getNickname() << NC << std::endl;
}

//Probleme de NickName a gerer: _ gerer pour un client, mais pas pour +...
void	Server::nick(int client_fd) {
	std::string	name					=	_msg.command.substr(5);
	Client		*currentUser			=	&_clients[_msg.currentIndex];
	currentUser->setOldname(currentUser->getNickname());

	// std::cout << "Verifying name :'" << name << "'\n\n";
	if (!currentUser->getRegistered())
	{
		currentUser->setNickname(name);
		currentUser->setOldname(name);
	}
	if (name.empty()) {
		_msg.response = printMessage("431", currentUser->getNickname(), " :No nickname given");}
	else if (!isValidNick(name)) {
		_msg.response = printMessage("432", currentUser->getNickname(), name + " :Erroneous nickname"); }
	else if (isAlreadyExists(name, client_fd, _clients)) {
		_msg.response = printMessage("433", currentUser->getNickname(), name + " :Nickname is already in use");
		currentUser->getNickname() = name; }
	else {
		setNick(currentUser, name); }

	// printListUser(_clients);

	_msg.response += ":" + currentUser->getOldname() + "!" + currentUser->getUsername() + "@localhost NICK " + currentUser->getNickname() + "\r\n";
}