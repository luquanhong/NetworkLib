/*
 * player_server.c
 *
 *  Created on: 2014Äê6ÔÂ5ÈÕ
 *      Author: Administrator
 */

#include "unixnet.h"


int main(int argc, char* argv){

	int ret;
	int reuse = 1;
	int listenFd, connFd, udpFd;

	int bConnect = 0;
	int bPlaying = 0;
	int bStopped = 0;

	int fd;
	int maxFd;
	fd_set rdFdset;
	int clientLen;
	struct sockaddr_in serverAddr, clientAddr;

	char recvBuffer[MAXLINE];
	char sendBuffer[MAXLINE];

	struct timeval timer;
	struct timeval* pTimer;

	timer.tv_sec =1;
	timer.tv_usec = 0;

	/* create tcp socket */
	listenFd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenFd < 0){
		perror("create listen socket error");
		exit(1);
	}

	/* set the server address&port */
	bzero(&serverAddr, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse,sizeof(int));

	ret = bind(listenFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(ret < 0){
		perror(" bind the listen socket to port error");
		exit(1);
	}

	listen(listenFd, LISTENQ);

	while(1){

		FD_ZERO(&rdFdset);

		if(bConnect == 0){

			FD_SET(listenFd, &rdFdset);
			maxFd = listenFd;
		}else if(bConnect == 1){

			FD_SET(connFd, &rdFdset);
			maxFd = max(connFd, maxFd);

			FD_SET(udpFd, &rdFdset);
			maxFd = max(maxFd, udpFd);

			/* if the stopped is true, set the timer is null*/
			if(bPlaying && !bStopped){

				pTimer = &timer;
			}else{

				pTimer = NULL;
			}
		}

		ret = select(maxFd + 1, &rdFdset, NULL, NULL, pTimer);

		switch(ret){

		case -1:
			perror("select error");
			exit(1);

		case 0:
			/* it like a timer send data per 1 second */
			/* the select time out */
			if(read(fd, sendBuffer, 1) != 0){

				ret = send(udpFd, sendBuffer, 1, 0);

				if(ret < 0 && errno != EINTR ){
					perror("send error");
					exit(1);
				}
			}else{

				close(connFd);
				close(udpFd);
				bConnect = 0;
			}

			pTimer = &timer;

			break;

		case 1:

			/* if it don't connection, a listen read is receive, create a connection */
			if(FD_ISSET(listenFd, &rdFdset) && bConnect != 1){

				clientLen = sizeof(clientAddr);
				connFd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientLen);

				if(connFd < 0 && errno != EINTR){

					perror("accept the client error");
					exit(1);
				}else if(connFd > 0){
					printf("server create a new connection\n");
				}

				clientAddr.sin_port = htonl(SERVER_PORT);

				udpFd = socket(AF_INET, SOCK_DGRAM, 0);
				ret = connect(udpFd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));

				if(ret < 0){

					perror("create upd connect error");
					exit(1);
				}else{

					bConnect = 1;

					fd = open("udp.text", O_RDWR);
				}
			}else if(FD_ISSET(connFd, &rdFdset)){

				ret = read(connFd, recvBuffer, 1);

				if(ret < 0 && errno != EINTR){

					perror("read from tcp connect error");
					exit(1);
				}

				switch(recvBuffer[0]){

				case 'p':
				case 'P':
					pTimer = &timer;
					bPlaying = 1;
					bStopped = 0;
					break;

				case 's':
				case 'S':
					pTimer = NULL;
					bStopped = 1;
					break;

				case 'e':
				case 'E':
					close(connFd);
					close(udpFd);
					close(fd);
					bConnect = 0;
					break;

				default:
					break;
				}
			}else if(FD_ISSET(udpFd, &rdFdset)){


				//ret = recv(udpFd, sendBuffer, 0);
			}

			break;

		}
	}
}

