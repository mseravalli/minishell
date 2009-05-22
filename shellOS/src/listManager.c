#include <stdio.h>
#include <stdlib.h>
#include "header.h"


void addToList(struct backgrNode **list){

	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = 666;
	tmpNode->next = *list;

	//printf("interno %d", (*list)->pid);



	*list = tmpNode;


}
void printList(struct backgrNode **list){

	struct backgrNode *tmpNode;
	tmpNode = *list;
	while(tmpNode != NULL){

		printf("interno %d\n", tmpNode->pid);
		tmpNode = tmpNode->next;

	}

}
