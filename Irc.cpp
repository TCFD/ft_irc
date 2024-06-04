#include "Irc.hpp"

Polls::Polls(int fd) : serverFd(fd)
{
    serverPollFds.fd = serverFd;
    serverPollFds.events = POLLIN;
    pollFds.push_back(serverPollFds);
}

void Polls::mainPoll(void)
{
    while (true) {
        pollCount = poll(pollFds.data(), pollFds.size(), -1);
        if (pollCount == -1)
            throw StrerrorException("Poll Error");
        for (size_t i = 0; i < pollFds.size(); i++){
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverFd) {
                    createClientPoll();
                }
            }
        }
    }
}
//Creer classe ClientPoll en lien avec Polls
void    Polls::createClientPoll(void)
{
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd == -1)
        perror("accept");
    fcntl(clientFd, F_SETFD, O_NONBLOCK);

    clientPollFds.fd = clientFd;
    clientPollFds.events = POLLIN;
    pollFds.push_back(clientPollFds);

    clientsBuffer[clientFd] = "";

    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
	Users temp;
    temp.indexInPollFd = pollFds.size() - 1;
	userTab.tab.push_back(temp);
}