#include <stdio.h>
#include <stdlib.h>
#include "header.h"


void addToList(int procID, char *cmd){
/*
	int i;
	for (i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid == 0){
			bckgrdList[i].pid = procID;
			bckgrdList[i].usedCommand = cmd;
			break;
		}
	}


	*/
	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;
	tmpNode->usedCommand = cmd;
	tmpNode->next = bckgrdList;

	bckgrdList = tmpNode;


}
void printList(){
/*
	int i;
	for (i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid != 0)
			printf("%d - %s\n", bckgrdList[i].pid, bckgrdList[i].usedCommand);
	}


	*/
	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;
	while(tmpNode != NULL){

		printf("%d - %s\n", tmpNode->pid, tmpNode->usedCommand);
		tmpNode = tmpNode->next;

	}


}
