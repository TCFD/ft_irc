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
void	Polls::nick() {
	std::string	name			=	msg.command.substr(5);
	User		*currentUser	=	&tab[msg.currentIndex];
	std::string	oldname			=	":" + currentUser->nickName;

	std::cout << "Verifying name :'" << name << "'\n\n";
	if (name.empty())
		msg.response = oldname + " 431 " + name + "\r\n";
	else if (!isValidNick(name)) {
		if (currentUser->newUser == true)
			msg.response = ":server 432 * " + name + " " +  name + " :\r\n";
		else
			msg.response = ":server 432 " + name + " :\r\n";
	}
	else {
		try {
				
			for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); ++it) {
				if (it->nickName == name && it->newUser == false)
					throw std::invalid_argument("");
			}
			
			std::cout << "changing name\n";
			currentUser->nickDone = true;
			if (currentUser->nickDone && currentUser->userDone)
				currentUser->newUser = false;
			currentUser->nickName = name;
			msg.response = oldname + " NICK " + name + "\r\n";
			std::cout << "List of known users : ";
			for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
				std::cout << it->nickName << ",";
			}
			std::cout << "\n";
	
		} catch (const std::invalid_argument& e) {
			if (currentUser->newUser == true)
				msg.response = ":server 433 * " + name + " " +  name + " :\r\n";
			else
				msg.response = ":server 433 " + currentUser->nickName + " " + name + " :Name already in use.\r\n";
		}
	}
}

//			response = prefix + "001 " + name +" :Welcome to the Internet Relay Network, " + name + "\r\n";