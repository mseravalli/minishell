#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


void addToList(int procID, char cmd[]){


	int i = 0;
	for(i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid == 0)
			break;
	}

	char value[MAX_LENGTH];

	value[0] = 256 % (i+140);
	value[1] = '\0';

	//strcpy( value, cmd );

	printf("inserting at the %dth position\n", i);
	bckgrdList[i].pid = procID;
	bckgrdList[i].usedCommand = value;


	/*
	char value[MAX_LENGTH];

	strcpy( value, cmd );


	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;
	tmpNode->usedCommand = value;
	tmpNode->next = bckgrdList;

	bckgrdList = tmpNode;
	*/


}


void printList(){

	int i = 0;
	for(i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid != 0)
			printf("%d - %s\n", bckgrdList[i].pid, bckgrdList[i].usedCommand);
	}

	/*
	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;
	while(tmpNode != NULL){

		printf("%d - %s\n", tmpNode->pid, tmpNode->usedCommand);
		tmpNode = tmpNode->next;

	}
	*/


}
