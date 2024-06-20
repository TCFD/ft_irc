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
	return (":server " + num + " " + nickname + " " + message + "\n");
}
//		return (_printMessage("001", this->_clients[i]->getNickName(), "Welcome to the Internet Relay Network " + this->_clients[i]->getID()));

void	Polls::setNick(User* currentUser, std::string name, std::string oldname) {

	std::cout << "changing name\n";
	currentUser->nickName = name;
	if (currentUser->userName != "") {
		currentUser->id = currentUser->nickName + "!" + currentUser->userName + "@" + currentUser->host;
		currentUser->registered = true;
		if (oldname.size() == 1)
			msg.response = printMessage("001", currentUser->nickName, " :Welcome to the Internet Relay Network " + currentUser->id);
		else
			msg.response = oldname + " NICK " + name + "\n";
	}
}

void	Polls::nick() {
	std::string	name					=	msg.command.substr(5);
	User		*currentUser			=	&tab[msg.currentIndex];
	std::string	oldname					=	":" + currentUser->nickName;

	std::cout << "COMMAND === " << msg.command << std::endl;
	std::cout << "Verifying name :'" << name << "'\n\n";
	if (name.empty())
		msg.response = printMessage("431", currentUser->nickName, ":No nickname given");
	else if (!isValidNick(name)) {
		msg.response = printMessage("432", currentUser->nickName, name + " :Erroneous nickname");
	}
	else {
		try {
			for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); ++it) {
				if (currentUser->registered && it->nickName == name)
					throw std::invalid_argument("");
				else if (!currentUser->registered && it->nickName == name)
					{setNick(currentUser, name+"_", oldname); msg.command += "_"; throw std::invalid_argument("");}
			}
			
			setNick(currentUser, name, oldname);

			std::cout << "List of known users : ";
			for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
				std::cout << it->nickName << ",";
			}
			std::cout << "\n";

		} catch (const std::invalid_argument& e) {
			msg.response = printMessage("443", currentUser->nickName, name + " :Nickname is already in use");
		}
	}
}

//			response = prefix + "001 " + name +" :Welcome to the Internet Relay Network, " + name + "\r\n";