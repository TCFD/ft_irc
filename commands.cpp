/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:26:55 by raphael           #+#    #+#             */
/*   Updated: 2024/06/06 14:21:13 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

std::string Polls::returnZeroOneEnd(User user) {
	return (user.userName + "!" + user.userName + "@localhost\r\n"); //! Attention, une fois integeree, le premier "username" devra etre le NICK, pas le USERNAME
}

void	Polls::nick(std::string & response, const std::string & command, std::string & prefix) {
	std::string name = command.substr(5);
	try {
		for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
			if (it->userName == name)
				throw std::invalid_argument("");
		}
		std::string oldname = tab[currentIndex].userName;
		tab[currentIndex].userName = name;
		if (oldname.empty())
			response = prefix + "001 " + name +" :Welcome to the Internet Relay Network, " + name + "\r\n";
		else
			response = prefix + "NICK " + name + " You changed your username to " + name + " " + returnZeroOneEnd(tab[currentIndex]);
	} catch (const std::invalid_argument& e) {
		response = prefix + "433 * " + name + " :Nickname is already in use\r\n";
	}
}