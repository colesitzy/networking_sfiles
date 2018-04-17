/*
	udp-server: UDP/IP sockets example
	keep reading data from the socket, echoing
	back the received data.

	usage:	echoserver [-d] [-p port]
*/

#include <stdio.h>
#include <stdlib.h>	/* needed for os x */
#include <string.h>	/* for memset */
#include <sys/socket.h>
#include <arpa/inet.h>	/* defines inet_ntoa */
#include <netinet/in.h>
#include <sys/errno.h>   /* defines ERESTART, EINTR */
#include <sys/wait.h>    /* defines WNOHANG, for wait() */
#include <iostream>
#include <unistd.h>

#ifndef ERESTART
#define ERESTART EINTR
#endif


using namespace std;
extern int errno;

void serve(int port);	/* main server function */
void disconn(void);

main(int argc, char **argv)
{

	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int port = 5077;
	static char usage[] = "usage: %s [-d] [-p port]\n";

	while ((c = getopt(argc, argv, "h::d:p:")) != -1){
		switch (c) {
		case 'h':
			cout << "options:" <<  endl;
			cout << "-h displays help" << endl;
			cout << "-d enables debugging features" << endl;
			cout << "-p port_number ... defaults to 5077" << endl;
			return 0;
			break;
		case 'p':
			port = atoi(optarg);
			if (port < 1024 || port > 65535) {
				fprintf(stderr, "invalid port number: %s\n", optarg);
				err = 1;
			}
			break;
		case '?':
			err = 1;
			break;
		}
	}
	serve(port);
}

/* serve: set up the service */

void
serve(int port)
{
	int svc;        /* listening socket providing service */
	int rqst;       /* socket accepting the request */
	socklen_t alen;       /* length of address structure */
	struct sockaddr_in my_addr;    /* address of this service */
	struct sockaddr_in client_addr;  /* client's address */
	int sockoptval = 1;
	char hostname[128]; /* host name, for debugging */

	gethostname(hostname, 128);

	//call socket

	if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		exit(1);
	}

	/* we use setsockopt to set SO_REUSEADDR. This allows us */
	/* to reuse the port immediately as soon as the service exits. */
	/* Some operating systems will not allow immediate reuse */
	/* on the chance that some packets may still be en route */

	setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));



	memset((char*)&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(svc, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
		perror("bind failed");
		exit(1);
	}

	if (listen(svc, 5) < 0) {
		perror("listen failed");
		exit(1);
	}
	char buff[10000];
	printf("server started on %s, listening on port %d\n", hostname, port);

	alen = sizeof(client_addr);

	for (;;) {
		while ((rqst = accept(svc,
		                (struct sockaddr *)&client_addr, &alen))
					 < 0)
			if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
				perror("accept failed");
				exit(1);
			}


		printf("received a connection from: %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
   		ssize_t nbytes = read(svc, buff, 10000);
		if(nbytes =< 0){
		cout << "read failed" << endl;
		}
		else{
		cout << nbytes << endl;
		}
		for(int i =0; i< 30; i++){
		cout <<ntohl(buff[i]) << endl;
		}

		shutdown(rqst, 2);    
	}
}
