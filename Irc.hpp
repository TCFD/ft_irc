#ifndef IRC_HPP
# define IRC_HPP

# include "Server.hpp"
# include "Client.hpp"

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
	std::string	id;
	std::string	host;
	bool		registered;
    int         operators;
};

struct Msg {
    std::string response;
    int	        currentIndex;
    std::string command;
    std::string currentChan;
    std::string prefix;
};

class Polls : public Server
{
    private:
        Msg msg;
        // std::string currentChannel;

		// int	currentIndex;
        std::map<std::string, std::string> channels;

		
		std::string returnZeroOneEnd(User user);

        std::vector<struct pollfd>		pollFds;
        struct pollfd					clientPollFds;
        struct pollfd					serverPollFds;
        int								serverFd;
        int								pollCount;
        std::map<int, std::string>		clientsBuffer;
		std::vector<User>				tab;

		User							findUser(std::string name);
		bool							isValidNick(const std::string& nick);
    public:
        Polls(void)						{};
        Polls(int fd);
        ~Polls(void)					{};

		void	handle_client_command(int client_fd);
        void    send_response(int client_fd);
		void	nick(int client_fd);
        void	setNick(User* currentUser, std::string name, std::string oldname);

        void    mainPoll(void);
        void    createClientPoll(void);
   		void	clientDisconnected(int bytes_received);
        
        int     channelHandle();
        int     modesHandle();

};

#endif