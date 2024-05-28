#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>


typedef struct User 
{
	char name[256];
	int number;
}User;

int main(void) {
	int	socketClient = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in adress;

	adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	adress.sin_family = AF_INET;
	adress.sin_port = htons(30000);

	if (connect(socketClient, (const struct sockaddr *)&adress, sizeof(adress)) == 0 ) {
		
		User user;
		// send(socketClient, &test, sizeof(test), 0);
		recv(socketClient, &user, sizeof(User), 0);
		std::cout << "Client says : We recieved " << user.name << ";" << user.number << std::endl;
	}
	else
		perror("connect failed: ");
	close(socketClient);
	return 0;
}