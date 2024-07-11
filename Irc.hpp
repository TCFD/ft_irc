#ifndef IRC_HPP
# define IRC_HPP

// # include "Server.hpp"
# include "Client.hpp"

# define MAP_TAB            std::map<std::string, int>
# define VEC_LIST           std::vector<std::string>
# define CHAN_VECTOR        std::vector<Channel>
# define USER_VECTOR        std::vector<User>
# define CHAN_ITERATOR      std::vector<Channel>::iterator
# define USER_ITERATOR      std::vector<User>::iterator

# define CLIENT_VEC  std::vector<Client>
# define CHAN_VEC    std::vector<Channel>

# define NC       "\033[0m"
# define BLACK       "\033[30m"      /* Noir */
# define RED         "\033[31m"      /* Rouge */
# define GREEN       "\033[32m"      /* Vert */
# define YELLOW      "\033[33m"      /* Jaune */
# define BLUE        "\033[34m"      /* Bleu */
# define MAGENTA     "\033[35m"      /* Magenta */
# define CYAN        "\033[36m"      /* Cyan */


std::string	printMessage(std::string num, std::string nickname, std::string message);

struct User {
	int			indexInPollFd;
	std::string	userName;
	std::string	nickName;
	std::string	realName;
	bool		newUser;
	bool		nickDone;
    VEC_LIST    modes;
    std::string oldName;
	std::string	id;
	std::string	host;
	bool		registered;
    int         operators;
    int         fd;
};

struct Msg {
    int	        currentIndex;
    int         currentChan;
    std::string response;
    std::string command;
    std::string prefixServer;
    std::string prefixNick;
};

struct Channel {
    std::string                 name;
    std::string                 pwd;
    VEC_LIST                    modes;
    MAP_TAB                     usersInChan;
};

class Polls : public Server
{
    private:
        // Msg msg;

        // std::vector<struct pollfd>		pollFds;
        // struct pollfd					clientPollFds;
        // struct pollfd					serverPollFds;
        // int								serverFd;
        // int								pollCount;
        std::map<int, std::string>		clientsBuffer;
		USER_VECTOR     				tab;
        CHAN_VECTOR                     tabChan;

		User							findUser(std::string name);
		bool							isValidNick(const std::string& nick);
    
    public:
        // Polls(void)						{};
        // Polls(int fd);
        // ~Polls(void)					{};

		void	        handle_client_command(int client_fd);
        void            send_response(int client_fd);
		void	        nick(int client_fd);
        void	        setNick(User* currentUser, std::string name);
        bool	        isAlreadyExists(std::string name, int clientFd);

        // void            mainPoll(void);
        // void            createClient(void);
   		// void	        clientDisconnected(int bytes_received);
        

        // Modes/Channels concerns
        int             channelHandle();
        int             modesHandle();
        void            sendToChan(void);
        void            modesOptions(VEC_LIST& split);
        void            errorModes(VEC_LIST& split);
        void            errorLenModes(VEC_LIST& split);

        // Useful MODE
        std::string     returnZeroOneEnd(User user);
        bool            isChanExists(std::string target);
        bool            isUserExists(std::string target);
        bool            isFourArgs(VEC_LIST& split);
        bool            isUserInChan(std::string target);
        bool            foundModeInChan(std::string mod, VEC_LIST modList);
        int             userInChanFd(std::string nick);
        
        void            modeK(VEC_LIST& split);
        void            modeO(VEC_LIST& split);

        VEC_LIST        cutModeCommand();

};

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