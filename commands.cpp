/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:26:55 by raphael           #+#    #+#             */
/*   Updated: 2024/06/10 11:26:52 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

void	Polls::nick(std::string & response, const std::string & command, std::string & prefix) {
	std::string name = command.substr(5);
	User	*currentUser = &tab[currentIndex];
	
	try {
		for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
			if (it->nickName == name)
				throw std::invalid_argument("");
		}
		std::cout << "changing name\n";
		std::string oldname = currentUser->nickName;
		currentUser->nickName = name;
		if (currentUser->newUser == true)
			currentUser->nickDone = true;
		else {
			response = prefix + "NICK " + name + " You changed your NickName to " + name + "\r\n";
		}
	} catch (const std::invalid_argument& e) {
		response = prefix + "433 * " + name + " :Nickname is already in use\r\n";
	}
}

//			response = prefix + "001 " + name +" :Welcome to the Internet Relay Network, " + name + "\r\n";