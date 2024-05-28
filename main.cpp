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

	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_family = AF_INET;
	adress.sin_port = htons(30000);

	bind(socketServer, (const struct sockaddr *)&adress, sizeof(adress));

	listen(socketServer, 5);

	int	socketClient = accept(socketServer, NULL, NULL);
	std::cout << "Server says : client accepted\n";
	char test[256];

	int bytes = recv(socketClient, test, 256, 0);
	while (bytes != 0 && bytes != -1) {
		std::cout << "Server says : We recieved " << test << std::endl;
		send(socketClient, test, bytes, 0);
		bytes = recv(socketClient, test, 256, 0);
	}

	close(socketServer);
	close(socketClient);
	return 0;
}