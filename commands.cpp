/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:26:55 by raphael           #+#    #+#             */
/*   Updated: 2024/06/07 12:50:51 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

std::string Polls::returnZeroOneEnd(User user) {
	return (user.userName + "!" + user.userName + "@localhost\r\n"); //! Attention, une fois integeree, le premier "username" devra etre le NICK, pas le USERNAME
}

void	Polls::nick(Msg& msg) {
	std::string name = msg.command.substr(5);
	try {
		for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
			if (it->userName == name)
				throw std::invalid_argument("");
		}
		std::string oldname = tab[msg.currentIndex].userName;
		tab[msg.currentIndex].userName = name;
		if (oldname.empty())
			msg.response = msg.prefix + "001 " + name +" :Welcome to the Internet Relay Network, " + name + "\r\n";
		else
			msg.response = msg.prefix + "NICK " + name + " You changed your username to " + name + " " + returnZeroOneEnd(tab[msg.currentIndex]);
	} catch (const std::invalid_argument& e) {
		msg.response = msg.prefix + "433 * " + name + " :Nickname is already in use\r\n";
	}
}