/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   commands.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: rciaze <rciaze@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/06/05 17:26:55 by raphael		   #+#	#+#			 */
/*   Updated: 2024/06/10 13:04:07 by rciaze		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Irc.hpp"

/*
* Erreurs pour la commande NICK :
//? ERR_NONICKNAMEGIVEN (431) : Aucun pseudonyme n'a été fourni.
//? ERR_ERRONEUSNICKNAME (432) : Le pseudonyme fourni contient des caractères non valides.
//? ERR_NICKNAMEINUSE (433) : Le pseudonyme est déjà utilisé par un autre utilisateur.
//! ERR_NICKCOLLISION (436) : Le pseudonyme est en collision avec un pseudonyme déjà utilisé sur un autre serveur.
//! ERR_UNAVAILRESOURCE (437) : Le pseudonyme est temporairement indisponible (par exemple, en raison de restrictions du serveur).
*/

bool Polls::isValidNick(const std::string& nick) {
	const std::string invalidChars = "@!#$%^&*()+=[]{}\\|:;'\"<>?,/";

	for (std::string::const_iterator it = nick.begin(); it != nick.end(); ++it) {
		if (std::isspace(*it) || invalidChars.find(*it) != std::string::npos) {
			std::cout << "Invalid name, detected invalid char :'" << *it << "'\n";
			return false;
		}
	}
	return true;
}

std::string	printMessage(std::string num, std::string nickname, std::string message)
{
	if (nickname.empty())
		nickname = "*";
	return (":localhost " + num + " " + nickname + " " + message + "\r\n");
}

void	Polls::setNick(User* currentUser, std::string name) {

	currentUser->oldName = currentUser->nickName;
	currentUser->nickName = name;
	if (currentUser->userName != "") {
		currentUser->id = currentUser->nickName + "!" + currentUser->userName + "@" + currentUser->host;
		currentUser->registered = true; }
	std::cout << MAGENTA "NAME ==> " << currentUser->nickName << NC << std::endl;
}

bool	Polls::isAlreadyExists(std::string name, int clientFd)
{
	std::cout << BLUE "FD required: " NC << clientFd << std::endl;
	for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
		std::cout << "nick: " << it->nickName << " and the client fd is : " << it->fd << std::endl;
		if (it->nickName == name && it->fd != clientFd)
			return true;
	}
	return false;
}

//Probleme de NickName a gerer: _ gerer pour un client, mais pas pour +...
void	Polls::nick(int client_fd) {
	std::string	name					=	msg.command.substr(5);
	User		*currentUser			=	&tab[msg.currentIndex];
	currentUser->oldName				=	currentUser->nickName;

	std::cout << "Verifying name :'" << name << "'\n\n";
	if (!currentUser->registered)
	{
		currentUser->nickName = name;
		currentUser->oldName = name;
	}
	if (name.empty()) {
		msg.response = printMessage("431", currentUser->nickName, " :No nickname given");}
	else if (!isValidNick(name)) {
		msg.response = printMessage("432", currentUser->nickName, name + " :Erroneous nickname"); }
	else if (isAlreadyExists(name, client_fd)) {
		msg.response = printMessage("433", currentUser->nickName, name + " :Nickname is already in use");
		currentUser->nickName = name; }
	else {
		setNick(currentUser, name); }

	std::cout << "List of known users : ";
	for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
		std::cout << it->nickName << ",";}
	std::cout << "\n";

	msg.response += ":" + currentUser->oldName + "!" + currentUser->userName + "@localhost NICK " + currentUser->nickName + "\r\n";

}
