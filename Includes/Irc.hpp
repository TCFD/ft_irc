#ifndef IRC_HPP
# define IRC_HPP

# define CHAN_VEC		   	std::vector<Channel>
# define CHAN_IT			CHAN_VEC::iterator
# define CLIENT_VEC		 	std::vector<Client>
# define CLIENT_IT		  	CLIENT_VEC::iterator
# define CHAR_VEC		  	std::vector<char>
# define MAP_TAB			std::map<std::string, int>
# define STR_VEC			std::vector<std::string>

# include <algorithm>
# include <arpa/inet.h>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <errno.h>
# include <exception>
# include <fcntl.h>
# include <iostream>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>

# include "Poll.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define NC			"\033[0m"
# define BOLD		"\033[1m"
# define BLACK		"\033[30m"	  /* Noir */
# define RED		"\033[31m"	  /* Rouge */
# define GREEN		"\033[32m"	  /* Vert */
# define YELLOW		"\033[33m"	  /* Jaune */
# define BLUE		"\033[34m"	  /* Bleu */
# define MAGENTA	"\033[35m"	  /* Magenta */
# define CYAN		"\033[36m"	  /* Cyan */

class Channel;

struct Msg {
	int			currentIndex;
	int		 	currentChan;
	std::string response;
	std::string command;
	std::string prefixServer;
	std::string prefixNick;
};

//Useful to commands
int	 			user_in_chan_fd(std::string nick, CLIENT_VEC clients);

bool			is_user_exists(std::string target, CLIENT_VEC clients);
bool			is_user_in_chan(std::string target, Channel chan);
bool			found_mode_in_chan(char mod, CHAR_VEC modList);
bool			is_user_an_operator(std::string target, Channel chan);
bool			is_four_args(STR_VEC& split);
bool			is_valid_nick(const std::string& nick);
bool			is_already_exists(std::string name, int clientFd, CLIENT_VEC clients);

std::string		int_to_str(int nb);
std::string		char_to_str(char nb);
std::string		time_to_str(time_t nb);
std::string		print_message(std::string num, std::string nickname, std::string message);

// DEBUG
void			print_list_user(CLIENT_VEC&   clients);

#endif