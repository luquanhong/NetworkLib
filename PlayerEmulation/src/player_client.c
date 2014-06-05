/*
 * player_client.c
 *
 *  Created on: 2014Äê6ÔÂ5ÈÕ
 *      Author: Administrator
 */

#include "unixnet.h"

int main(int argc, char* argv[]){

	int n;
	int ret;
	int connFd, udpFd;

	int maxFd;
	fd_set rdSet;

	char sendBuffer[MAXLINE];
	char recvBuffer[MAXLINE];

	struct timeval timer;
	struct timeval* pTimer;

	struct sockaddr_in clientAddr, serverAddr;

	struct sockaddr_in addr;
	socklen_t	addrlen;

	/* for udp connect*/
	bzero(&clientAddr, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(SERVER_PORT);
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	udpFd = socket(AF_INET, SOCK_DGRAM, 0);

	ret = bind(udpFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

	/* for tcp connect*/
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_aton(argv[1], &serverAddr.sin_addr);

	connFd = socket(AF_INET, SOCK_STREAM, 0);

	ret = connect(connFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	timer.tv_sec =1;
	timer.tv_usec =0;

	pTimer = NULL;

	while(1){

		FD_ZERO(&rdSet);

		FD_SET(connFd, &rdSet);
		FD_SET(0, &rdSet);
		maxFd = connFd;

		FD_SET(udpFd, &rdSet);
		maxFd = max(maxFd, udpFd);

		ret = select(maxFd + 1, &rdSet, NULL, NULL, pTimer);

		switch(ret){

		case 0:
			break;
		case -1:

			exit(1);
		default:

			if(FD_ISSET(0, &rdSet)){

				ret = read(0, sendBuffer, 2);

				if(ret <0 && errno != EINTR){
					exit(1);
				}else if(ret == 0){

					close(connFd);
					close(udpFd);
					exit(0);
				}else{

					ret = write(connFd, sendBuffer, 1);
					if(ret <0 && errno != EINTR){
						exit(1);
					}
				}
			}else if(FD_ISSET(udpFd, &rdSet)){

				n = recvfrom(udpFd, recvBuffer, MAXLINE, 0, (struct sockaddr*)&addr, &addrlen);
				recvBuffer[n] = '\0';
				fprintf(stdout, "recv data: %s \n", recvBuffer);

			}else if(FD_ISSET(connFd, &rdSet)){

			}

			break;
		}
	}
}
