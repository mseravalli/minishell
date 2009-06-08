#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void addToList(int procID, char cmd[], char inRes[], char outRes[]){


	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;
	strcpy( tmpNode->usedCommand, cmd );

	strcpy( tmpNode->inResource, inRes );
	strcpy( tmpNode->outResource, outRes );

	tmpNode->next = bckgrdList;

	bckgrdList = tmpNode;


}


void deleteFromList(int procID){

	struct backgrNode *tmpNode;
	tmpNode= malloc(sizeof(struct backgrNode));
	tmpNode->next = bckgrdList;

	if(bckgrdList->pid == procID){
		bckgrdList = bckgrdList->next;
	}else{
		while(tmpNode->next != NULL){

			if(tmpNode->next->pid == procID){
				tmpNode->next->pid = 0;
				tmpNode->next = tmpNode->next->next;
			}
			tmpNode = tmpNode->next;

		}
	}

}


void cleanList(){

	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;

	FILE * processesState;
	char fileLocation[50];
	char pidToFind[5];
	char pState[MAX_LENGTH];

	strcpy(fileLocation, "/proc/");

	while(tmpNode != NULL){



		if(kill(tmpNode->pid, 0) == -1){
			deleteFromList(tmpNode->pid);
		} else {
			sprintf(pidToFind, "%d", tmpNode->pid);

			strcat(fileLocation, pidToFind );
			strcat(fileLocation, "/stat" );


			processesState = fopen(fileLocation, "r");

			fscanf(processesState, "%s", pState);
			fscanf(processesState, "%s", pState);
			fscanf(processesState, "%s", pState);

			if(strcmp("Z", pState) == 0){
				deleteFromList(tmpNode->pid);
			}


			fclose(processesState);

		}

		tmpNode = tmpNode->next;


	}

}


void printList(){

	cleanList();

	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;


	while(tmpNode != NULL){

		printf("%d - %s \t%s \t stdin: %s \t stdout: %s\n", tmpNode->pid, tmpNode->usedCommand, tmpNode->pState, tmpNode->inResource,  tmpNode->outResource);
		fflush(stdout);

		tmpNode = tmpNode->next;
	}




}



