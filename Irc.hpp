#ifndef IRC_HPP
# define IRC_HPP

# include "Server.hpp"
# include "Client.hpp"

struct User {
	int         indexInPollFd;
	std::string userName;
    std::string nickName;
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
        std::vector<struct pollfd> pollFds;
        struct pollfd clientPollFds;
        struct pollfd serverPollFds;
        int serverFd;
        int pollCount;
        Msg msg;
        // std::string currentChannel;

		// int	currentIndex;
        std::map<int, std::string> clientsBuffer;
		std::vector<User> tab;
        std::map<std::string, std::string> channels;

		
		User	findUser(std::string name);
		std::string returnZeroOneEnd(User user);

    public:
        Polls(void) {};
        Polls(int fd);
        ~Polls(void) {};

		void	handle_client_command(int client_fd);
        void    send_response(int client_fd, Msg& msg);
		void	nick(Msg& msg);

        void    mainPoll(void);
        void    createClientPoll(void);
   		void	clientDisconnected(int bytes_received);
        
        int     channelHandle(Msg& msg);
        int     modesHandle(Msg& msg);

};

#endif