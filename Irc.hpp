#ifndef IRC_HPP
# define IRC_HPP

// # include "Server.hpp"
# include <cstdlib>
# include <cstdio>
# include <fcntl.h>
# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <map>
# include <vector>
# include <exception>
# include <errno.h>
# include <algorithm>
# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Channel;

# define CLIENT_VEC         std::vector<Client>
# define MAP_TAB            std::map<std::string, int>
# define CHAR_LIST          std::vector<char>
# define STR_LIST           std::vector<std::string>
# define CHAN_VEC           std::vector<Channel>

# define NC          "\033[0m"
# define BLACK       "\033[30m"      /* Noir */
# define RED         "\033[31m"      /* Rouge */
# define GREEN       "\033[32m"      /* Vert */
# define YELLOW      "\033[33m"      /* Jaune */
# define BLUE        "\033[34m"      /* Bleu */
# define MAGENTA     "\033[35m"      /* Magenta */
# define CYAN        "\033[36m"      /* Cyan */

// struct User {
// 	int			indexInPollFd;
// 	std::string	userName;
// 	std::string	nickName;
// 	std::string	realName;
// 	bool		newUser;
// 	bool		nickDone;
//     VEC_LIST    modes;
//     std::string oldName;
// 	std::string	id;
// 	std::string	host;
// 	bool		registered;
//     int         operators;
//     int         fd;
// };

struct Msg {
    int	        currentIndex;
    int         currentChan;
    std::string response;
    std::string command;
    std::string prefixServer;
    std::string prefixNick;
};

//Useful to commands
std::string	printMessage(std::string num, std::string nickname, std::string message);

bool    isUserExists(std::string target, CLIENT_VEC clients);
bool    isUserInChan(std::string target, CHAN_VEC channels, int currentChan);
bool    foundModeInChan(std::string mod, CHAR_LIST modList);
bool    isFourArgs(STR_LIST& split);
int     userInChanFd(std::string nick);

bool    isValidNick(const std::string& nick);
bool	isAlreadyExists(std::string name, int clientFd, CLIENT_VEC clients);

// DEBUG
void    printListUser(CLIENT_VEC&   clients);

// TEMPLATES

/* Make a message to print channel modes, or not */
template <typename V>
std::string    printModes(V& modes)
{
    std::string currentModes="+";
    for (typename V::iterator it=modes.begin(); it != modes.end(); ++it) {
        currentModes += *it; }
    if (currentModes.length() == 1)
        currentModes = ":No modes are set";
    return (currentModes);
};

#endif