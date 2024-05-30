#ifndef IRC_HPP
# define IRC_HPP

#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <map>
#include <vector>
#include "Socket.hpp"

class Socket;


// UTILS
void send_response(int client_fd, const std::string& response);

// CLIENTS
void handle_client_command(int client_fd, const std::string& command, Socket test);

#endif