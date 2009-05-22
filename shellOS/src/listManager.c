#include <stdio.h>
#include <stdlib.h>
#include "header.h"


void addToList(struct backgrNode **list, int procID, char *cmd){

	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;
	tmpNode->usedCommand = cmd;
	tmpNode->next = *list;

	*list = tmpNode;


}
void printList(struct backgrNode **list){

	struct backgrNode *tmpNode;
	tmpNode = *list;
	while(tmpNode != NULL){

		printf("%d - %s\n", tmpNode->pid, tmpNode->usedCommand);
		tmpNode = tmpNode->next;

	}

}
