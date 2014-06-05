/*
 * unixnet.h
 *
 *  Created on: 2014Äê6ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef UNIXNET_H_
#define UNIXNET_H_

#include <sys/types.h> 	/* basic system data types*/
#include <sys/socket.h> /* basic socket define*/
#include <sys/time.h>	/* timeval for select */
#include <time.h>		/* timespec for pselect */
#include <netinet/in.h> /* sockaddr_in struct, this is a tcp/ip internet socket*/
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <pthread.h>

/* for ipc */
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>


#define SERVER_PORT 9999
#define LISTENQ 10
#define MAXLINE 1024

#undef max
#define max(x, y) ( (x) > (y) ? (x) : (y) )

#endif /* UNIXNET_H_ */
