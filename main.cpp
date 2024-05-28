#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef struct User 
{
	char name[256];
	int number;
}User;

int main(void) {
	int	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in adress;

	adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	adress.sin_family = AF_INET;
	adress.sin_port = htons(30000);

	bind(socketServer, (const struct sockaddr *)&adress, sizeof(adress));

	listen(socketServer, 5);

	struct sockaddr_in client;
	socklen_t csize = sizeof(client);
	int	socketClient = accept(socketServer, (struct sockaddr *)&client, &csize);
	std::cout << "Server says : client accepted\n";
	User user {
		.name = "test",
		.number = 1
	};

	// recv(socketClient, test, sizeof(test), 0);
	// std::cout << "Server says : We recieved " << (std::string)*test << std::endl;
	send(socketClient, &user, sizeof(User), 0);

	close(socketServer);
	close(socketClient);
	return 0;
}