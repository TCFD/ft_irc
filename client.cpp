#include "Irc.hpp"

void handle_client_command(int client_fd, const std::string& command, Socket test) {
	std::string response;
	std::string prefix = ":localhost ";

	if (command.rfind("NICK", 0) == 0) {
		response = prefix + "001 Welcome to the IRC server!\r\n";
	} else if (command.rfind("USER", 0) == 0) {
		response = prefix + "001 Welcome, your user information is received.\r\n";
	} else if (command.rfind("PING", 0) == 0) {
		response = "PONG :" + command.substr(5) + "\r\n";
	} else if (command.rfind("CAP", 0) == 0) {
		response = "CAP * LS :\r\n"; // Simplement ignorer CAP pour cette implémentation
	} else if (command.rfind("JOIN", 0) == 0) {
		response = prefix + "JOIN " + command.substr(5) + "\r\n";
		test.saloon[command.substr(5)].push_back(client_fd);
		std::cout << command.substr(5) << " now has new client registered\n";
	}
	else if (command.rfind("PRIVMSG", 0) == 0) {
		std::string message = command.substr(command.find(':'));
		std::string	channel = command.substr(8, command.find(':') - 9);
		std::cout << "user wants to send " << message << " to " << channel<< std::endl;

		//:alice!~alice@host PRIVMSG #discussion :Bonjour tout le monde!

		std::string end_msg = ":username!~username@host PRIVMSG " + channel + " :" + message + "\r\n";
		for (std::vector<int>::iterator it = test.saloon[channel].begin(); it != test.saloon[channel].end(); ++it) {
			if (*it != client_fd) {
				send_response(*it, end_msg);
			}
		}	

	}
	else {
		response = prefix + "421 " + command.substr(0, command.find(' ')) + " :Unknown command\r\n";
	}

	send_response(client_fd, response);
}