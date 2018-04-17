#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "defaults.hpp"
#include "structure.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>


using namespace std;

int main(int argc, char **argv)
{
	struct sockaddr_in myaddr, remaddr;
	int fd, slen=sizeof(remaddr);
	char *server = "127.0.0.1";

	int c = 0;
	int dataNum = NUMBER_OF_DATAGRAMS;
	int port = PORT_NUMBER;
	char *login_name =(char *) "csitzberger";
	ClientDatagram *cd;
	size_t payload_length = strlen(login_name) + 1;
	size_t client_datagram_length = sizeof(ClientDatagram) + payload_length;

	cd->payload_length = payload_length;
	//char *buff[}
	//extern char *optarg;
	//extern int optint;
	//cout <<" check" <<endl;

	//add file compatability
	while ((c = getopt(argc, argv, "h::s:p:")) !=-1){
	switch (c) {

	case 'h':
		cout << ("./client options:") << endl;
		cout << ("-h displays help") << endl;
		cout << ("-s server address ... defaults to 127.0.0.1") << endl;
		cout << ("-p port number ... defaults to 39390") << endl;

		return 0;
		break;

	case 's':
		server = optarg;
		cout << "server address set to" << optarg << endl;
		break;
	case 'p':
		port = atoi(optarg);
		if (port < 1024 || port > 65535) {
			cout <<"" << port << " is an invalid port number" << endl;
		}
		break;
	}
	}
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0))!=-1)
	printf("socket created\n");
	else
	cout << "failed to  create socket"<< endl;

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
	else{
	cout << "bind successful" << endl;
	}



	//set where the packets are going
	//inet_aton converts IP to binary

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(port);
	if (inet_aton(server, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}



	//build leaving packet
	//int i = 0
	int packets_sent;
	for(int i=0; i < NUMBER_OF_DATAGRAMS; i++){
	cd->sequence_number = htonl(i);
	cd->payload_length = htons(cd->payload_length);
	//ClientDatagram *clientdata = cd;
	packets_sent = i;
	if (sendto(fd, (cd, login_name), client_datagram_length, 0, (struct sockaddr *)&remaddr, slen)==-1)
	{		perror("sendto");

	}

	else{
	//cout << "Sending packet " << i <<" to "<< server <<" port "<< port << endl;

	//	recvlen = recvfrom(fd, buf, BUFLEN, m (struct sockaddr *)&remaddr, &slen;
	//		if(recvlen >= 0) {
	//			buf[recvlen] = 0;
	//	}		printf("received message: \"%s\"|n", buf);
		}
	}
	cout << packets_sent << " packets sent" << endl;
	close(fd);

	return 0;

}




