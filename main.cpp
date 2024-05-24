#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <iostream>

int main(void) {
	char name[265];

	struct pollfd mypoll;
	
	memset(&mypoll, 0, sizeof(mypoll));
	mypoll.fd = 0;
	mypoll.events = POLLIN;

	std::cout << "Type in your name\n";

	if (poll(&mypoll, 1, 1000) == 1) {
		read(0, name, sizeof(name));
		std::cout << "Hello, " << name << "\n";
	}
	std::cout << "It took you ms to type in your name\n";
}