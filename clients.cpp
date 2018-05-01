#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "packets.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>


using namespace std;

void initializePacket(CPacket cp){
	cp.convo_id = 0;
	cp.seq_num = 0;
	cp.op_code = 0;
	cp.msg_len = 0;
	cp.window_index = 0;


}

int main(int argc, char **argv)
{
	struct sockaddr_in myaddr, remaddr;
	int fd, slen=sizeof(remaddr);
	char *server = "127.0.0.1";
	char buf[CP_BUFFER_SIZE];
	int c = 0;
	int port = PORT_NUMBER;
	int window_size = DEF_WINDOW_SIZE;
	string filepath;
	//extern char *optarg;
	//extern int optint;
	//cout <<" check" <<endl;

	//add file compatability
	while ((c = getopt(argc, argv, "h::f:n:s:p:")) !=-1){
	switch (c) {

	case 'h':
		cout << ("clientoptions:") << endl;
		cout << ("-f set filename") <<endl;
		cout << ("-h displays help") << endl;
		cout << ("-n window size ... defaults to 8") << endl;
		cout << ("-s server address ... defaults to 127.0.0.1") << endl;
		cout << ("-p port number ... defaults to 9990") << endl;
		break;
	case 'f':
		filepath = optarg;
	//	ifstream in(optarg);
	//	if(in.is_open()){
	//		infile = in;
	//		cout << "file is good" << endl;
	//	}
	//	else{
	//	cout << "bad file path" << endl;

		break;
	case 'n':
		window_size = atoi(optarg);
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
//	cout<<"Check one"<<endl;
//creates socket
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


	CPacket outpacket;

	initializePacket(outpacket);
	//ifstream infile(filepath);
	//if(infile.is_open()){
	//sequence loop
	//z = 0;
	//while(infile.get){
	//outpacket.msg[z] = infile.get;

	//slidingPaneLoop
		for(int i=0; i < window_size; i++) {
			outpacket.seq_num = window_size;
		//set cpacket op code
			if(i==0){
				outpacket.op_code = COP_CODE_START;
			}

			else if(i==(window_size-1)){
				outpacket.op_code = COP_CODE_FINISH;
			}
			else{
				outpacket.op_code = COP_CODE_DATA;
			}

		//send

//		if (sendto(fd, outpacket, 133, 0, (struct sockaddr *)&remaddr, slen)==-1)
//			perror("sendto");

		if (sendto(fd, buf, strlen(buf) , 0, (struct sockaddr *)&remaddr, slen)==-1)
			perror("sendto");



		else
		cout << "Sending packet " << i <<" to "<< server <<" port "<< port << endl;

	//	recvlen = recvfrom(fd, buf, BUFLEN, m (struct sockaddr *)&remaddr, &slen;
	//		if(recvlen >= 0) {
	//			buf[recvlen] = 0;
	//	}		printf("received message: \"%s\"|n", buf);
		}
	close(fd);
	}

	return 0;

}


