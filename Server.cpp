# include "Server.hpp"

Server::Server(int _port) : port(_port)
{
    socketDataSet();
}

void    Server::socketDataSet(void)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        throw StrerrorException("Socket Error");
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    { throw StrerrorException("Set Socket Options Error"); close (serverSocket); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    memset(&(serverAddr.sin_zero), '\0', 8);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    { throw StrerrorException("Bind Error"); close(serverSocket); }

    if (listen(serverSocket, 10) == -1)
    { throw StrerrorException("Listen Error"); close(serverSocket); }

    fcntl(serverSocket, F_SETFL, O_NONBLOCK);
    std::cout << "on est la !!" << std::endl;
}

// ServerPoll::ServerPoll(int serverFd) {
//     serverPollFds.fd = serverFd;
//     serverPollFds.events = POLLIN;
// }