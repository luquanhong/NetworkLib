/*
 * output.c
 *
 *  Created on: 2014��6��5��
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
