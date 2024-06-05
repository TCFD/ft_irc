/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphael <raphael@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:26:55 by raphael           #+#    #+#             */
/*   Updated: 2024/06/05 17:27:21 by raphael          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

void	Polls::nick(std::string & response, const std::string & command, std::string & prefix) {
	response = prefix + "001 ";
	std::string name = command.substr(5);
	try {
		for (std::vector<User>::iterator it = tab.begin(); it < tab.end(); it++) {
			if (it->userName == name)
				throw std::invalid_argument("");
		}
		if (tab[currentIndex].userName.empty())
			response += name + " Welcome to the IRC server!\r\n";
		else
			response += "You changed your username to " + name;
		tab[currentIndex].userName = name;
	} catch (const std::invalid_argument& e) {
		if (tab[currentIndex].userName.empty()) {
				tab[currentIndex].userName = name + "0";
			response = prefix + "001 " + " Welcome. Your name was already taken, so you got the name " + tab[currentIndex].userName + e.what();
		}
		else
			response = prefix + "433 " + tab[currentIndex].userName + " " + name + " " + ": Nickname is already in use";
	}
}