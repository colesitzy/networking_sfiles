

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "defaults.hpp"
#include "structure.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

#define BUFSIZE 2048


int
main(int argc, char **argv)
{
	struct sockaddr_in myaddr;
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);
	int recvlen;
	int fd;
	int port = PORT_NUMBER;
	unsigned char buf[BUFSIZE];
	bool verboseOn = false;
	bool debugModeOn = false;
	int c=0;

	while((c = getopt(argc, argv, "h::d:p:v:")) !=-1){
	switch (c) {
		case 'h':
			cout << "-s server options" << endl;
			cout << "-d debug mode" << endl;
			cout << "-p port number ... defaults to 127.0.0.1" << endl;
			cout << "-v verbose mode ... makes the server print payloads" << endl;
			return 0;
			break;
		case 'd':
			debugModeOn = true;
			cout << "Debug on" << endl;
			break;
		case 'p':
			port = atoi(optarg);
			cout << "Port number switched to " << port <<" " << endl;
			break;
		case 'v':
			verboseOn = true;
			cout << "Verbose on " << endl;
		}

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	for (;;) {
		cout << "waiting on port " << port << endl;
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) {
			buf[recvlen] = 0;
		}
		else
			printf("receive error\n");
		//const char *stlb = strlen(buf);
		//cout << "sending response" << endl;
		//if (sendto(fd, buf, stlb, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
		//	perror("sendto");
	}
}
}
