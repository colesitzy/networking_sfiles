

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

int conn(char *host, int port,  char *filepath);
void disconn(void);
ifstream inFile;

main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0;
	char *prompt = 0;
	int port = 5077;
	char *host = "127.0.0.1";	/* default: this host */
	char *filepath;
	static char usage[] = "usage: %s [-d] [-h serverhost] [-p port]\n";
	bool debugOn = false;

	while ((c = getopt(argc, argv, "h::d:fs:p:")) != -1){
		switch (c) {
		case 'h':
			cout << "Options:" << endl;
			cout << "-h displays help" << endl;
			cout << "-f file to send (must be a text file)" << endl;
			cout << "-s server_address ... defaults to 127.0.0.1" << endl;
			cout << "-p port_number ... defaults to 5077" << endl;
			return 0;
			break;
		case 'd':
			cout << "debug turned on" << endl;
			debugOn = true;
		case 'f':
			filepath = optarg;
			inFile.open(filepath);
			if(inFile.good() == false)
			{
				cout << "bad filepath" << endl;
				return 1;
			}
			break;
		case 's':
			host = optarg;
			cout << "Server address set to " << host << endl;
			break;
		case 'p':  /* port number */
			port = atoi(optarg);
			if (port < 1024 || port > 65535) {
				fprintf(stderr, "invalid port number: %s\n", optarg);
				err = 1;
			}
			break;

		}
	}

	printf("connecting to %s, port %d\n", host, port);

	if (!conn(host, port, filepath))    /* connect */
		exit(1);   /* something went wrong */

	disconn();    /* disconnect */
	return 0;
}

int fd;


int conn(char *host, int port, char *filepath )
{
	ifstream inFile;
	String readfile;
	struct hostent *hp;
	unsigned int alen;
	struct sockaddr_in myaddr;
	struct sockaddr_in servaddr;




	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}


	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

        alen = sizeof(myaddr);
        if (getsockname(fd, (struct sockaddr *)&myaddr, &alen) < 0) {
                perror("getsockname failed");
                return 0;
        }
	printf("local port number = %d\n", ntohs(myaddr.sin_port));


	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "could not obtain address of %s\n", host);
		return 0;
	}







	long filelen = readfile.lenght

	
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	if (connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect failed");
		return 0;
	}
	return 1;
}

void
disconn(void)
{
	printf("disconn()\n");
	shutdown(fd, 2);    /* 2 means future sends & receives are disallowed */
}
