/*
 * output.c
 *
 *  Created on: 2014Äê6ÔÂ5ÈÕ
 *      Author: Administrator
 */


#include <stdio.h>


int main(){


	char recvBuffer[100];

	while(fgets(recvBuffer, 100, stdin) != NULL){
		fprintf(stdout, "%s", recvBuffer);
	}

	return 0;
}
