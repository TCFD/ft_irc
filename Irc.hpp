#ifndef IRC_HPP
# define IRC_HPP

# include "Server.hpp"
# include "Client.hpp"


# define VEC_LIST           std::vector<std::string>
# define CHAN_VECTOR        std::vector<Channel>
# define USER_VECTOR        std::vector<User>
# define CHAN_ITERATOR      std::vector<Channel>::iterator
# define USER_ITERATOR      std::vector<User>::iterator

struct User {
	int			indexInPollFd;
	std::string	userName;
	std::string	nickName;
	std::string	realName;
	bool		newUser;
	bool		nickDone;
    int         operators;
    VEC_LIST modes;
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
    VEC_LIST                    usersInChan;
};

class Polls : public Server
{
    private:
        Msg msg;
        // std::map<std::string, std::string> channels;

		std::string returnZeroOneEnd(User user);

        std::vector<struct pollfd>		pollFds;
        struct pollfd					clientPollFds;
        struct pollfd					serverPollFds;
        int								serverFd;
        int								pollCount;
        std::map<int, std::string>		clientsBuffer;
		USER_VECTOR     				tab;
        CHAN_VECTOR                     tabChan;

		User							findUser(std::string name);
    public:
        Polls(void)						{};
        Polls(int fd);
        ~Polls(void)					{};

		void	handle_client_command(int client_fd);
        void    send_response(int client_fd);
		void	nick();

        void    mainPoll(void);
        void    createClientPoll(void);
   		void	clientDisconnected(int bytes_received);
        
        int             channelHandle();
        int             modesHandle();
        void            errorModes(VEC_LIST split);
        void            errorLenModes(VEC_LIST& split);

        //Useful MODE
        bool            isChanExists(std::string target);
        bool            isUserExists(std::string target);
        VEC_LIST        cutModeCommand();

};

#endif