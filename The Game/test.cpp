#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <sys/socket.h>

struct peos {
	int a;
	char b;
	double c;

	peos() {}
	peos(int a, char b, double c): a(a), b(b), c(c) {}

	// copy constructor
	peos(peos& p) {
		a = p.a;
		b = p.b;
		c = p.c;
	}

	// copy assignment
	peos& operator=(peos& p) {
		a = p.a;
		b = p.b;
		c = p.c;
		return *this;
	}
};

void receivePacket(char* buf, size_t size) {
	// read from socket
	int packetType = buf[0];

	switch(packetType) {
		case 1: {
			peos* p = new peos;
			memcpy(p, buf + 1, sizeof(peos));
			break;
		}
	}
}

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

	std::shared_ptr<peos> p = std::make_shared<peos>(1, 'a', 3.14);

	packageCreate(fd, p.get(), sizeof(peos));

	void* buf =

	listen(fd, );

	return 0;
}
