#pragma once

# include "Irc.hpp"

class Client;
class Server;


class Polls
{
    protected:
        std::vector<struct pollfd>		_pollFds;
        struct pollfd					_clientPollFds;
        struct pollfd					_serverPollFds;
        int								_pollCount;
        std::map<int, std::string>		_clientsBuffer;

    public:
        Polls() {};
        Polls(int fd);
        ~Polls() {};

        // int                         getPollCount() { return(_pollCount); };
        // std::vector<struct pollfd>  getPollsFds(void) {return (_pollFds); };
        // void                        setPollCount(int count) {_pollCount = count; };
        void            mainPoll(Server& server);
        void            addClientPoll(int clientFd);

};

class Server: public Client
{
    private:
        int                             _port;
        int                             _serverSocket;
        struct sockaddr_in              _serverAddr;
        int                             _limitUsers;
        Msg                             _msg;
        CLIENT_VEC                      _clients;
        CHAN_VEC                        _channels;
        Polls                           _poll;

    public:
        Server(int port);
        ~Server(void) {};

    // Getters / Setters
        int             getPort(void) {return(_port); };
        int             getServerSocket(void) { return(_serverSocket); };
        Msg             getMsg() {return(_msg); };
        void            setMsgIdx(int idx) {_msg.currentIndex = idx; };
        void            setMsgCmd(std::string cmd) {_msg.command = cmd; };
        void            setMsg() {_msg.currentChan = 0; _msg.prefixServer = ":server "; };
        void            setPoll(Polls& poll) {_poll = poll; };
        
        void            socketDataSet(void);

        int             createClient(Polls &poll);
   		void	        clientDisconnected(int bytes_received, int id);

        void            handleClientCommand(int client_fd);
        void            sendResponse(int client_fd);
        STR_LIST        cutModeCommand(void);

    /* * * Command * * */
    // MODE
        int             modesHandle(void);
        void            errorModes(STR_LIST& split);
        void            errorLenModes(STR_LIST& split);
        void            modesOptions(STR_LIST& split);
        void            modeK(STR_LIST& split);
        // void            modeO(STR_LIST& split);

    // NICK
        void            nick(int client_fd);
        void	        setNick(Client* currentUser, std::string name);
    // JOIN
        int             channelHandle(void);
        // void            sendToChan(void);
        bool            isChanExists(std::string target);

};

class StrerrorException : public std::exception
{
    private:
        char bufferMessage[256];

    public :
        explicit StrerrorException(const char* message){
            snprintf(bufferMessage, sizeof(bufferMessage), "%s: %s", message, strerror(errno)); }

        virtual const char* what() const throw(){
            return bufferMessage; };
};

