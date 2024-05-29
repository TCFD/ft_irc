#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

char *substr(char *str, int start, int end)
{
	char *buf;
	buf = (char *)malloc(end - start + 1);
	int j = 0;
	if (start > end)
		return NULL;
	while (str[start] && start < end) {
		buf[j] = str[start]; start++; j++;
	}
	buf[j] = 0;
	std::cout << buf << std::endl;
	return buf;
}

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

	memset(test, 256, 0);
	int bytes = recv(socketClient, test, 256, 0);
	while (bytes != 0 && bytes != -1) {
		char *result = substr(test, 0, bytes);
			std::cout << "Server recieved : " << substr(test, 0, bytes) << std::endl;
			send(socketClient, test, bytes, 0);
			memset(test, 256, 0); 	
			bytes = recv(socketClient, test, 256, 0);
			send(socketClient, "CAP * LS :multi-prefix sasl\r\n", 28, 0);
			send(socketClient, "CAP * ACK multi-prefix\r\n", 23, 0);
	}

	close(socketServer);
	close(socketClient);
	return 0;
}