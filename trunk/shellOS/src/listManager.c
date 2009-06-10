#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void addToList(int procID,int procGID, char cmd[], char inRes[], char outRes[]){


	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;
	tmpNode->pgid = procGID;

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

	if(bckgrdList != NULL){
		//if it is the first element
		if(bckgrdList->pid == procID){

			bckgrdList = bckgrdList->next;
		}

	}


	while(tmpNode != NULL){

		if(tmpNode->next != NULL){

			if(tmpNode->next->pid == procID){
				tmpNode->next = tmpNode->next->next;
			}

		}

		tmpNode = tmpNode->next;
	}

}


void updateList(){

	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;

	FILE * processesState;
	char fileLocation[50];
	char pidToFind[5];
	char pState[MAX_LENGTH];

	while(tmpNode != NULL){



		//printf("%d - %d\n", tmpNode->pid, kill(tmpNode->pid, 0));
		if(kill(tmpNode->pid, 0) == -1){
			deleteFromList(tmpNode->pid);

		} else {
			strcpy(fileLocation, "/proc/");
			sprintf(pidToFind, "%d", tmpNode->pid);
			strcat(fileLocation, pidToFind );
			strcat(fileLocation, "/stat" );

			processesState = fopen(fileLocation, "r");

			fscanf(processesState, "%s", pState);
			fscanf(processesState, "%s", pState);
			fscanf(processesState, "%s", pState);

			if(strcmp(pState, "Z") == 0){
				deleteFromList(tmpNode->pid);
			}else{
				strcpy(tmpNode->pState, pState);
			}

			fclose(processesState);
		}

		tmpNode = tmpNode->next;

	}

}


void printList(){

	updateList();

	struct backgrNode *tmpNode;

	tmpNode = bckgrdList;

	while(tmpNode != NULL){

		printf("%d - %s \t%s \t stdin: %s \t stdout: %s\n", tmpNode->pid, tmpNode->usedCommand, tmpNode->pState, tmpNode->inResource,  tmpNode->outResource);
		fflush(stdout);

		tmpNode = tmpNode->next;
	}




}

void notifyDeath(){

	struct backgrNode *tmpNode;
		tmpNode = bckgrdList;

		FILE * processesState;
		char fileLocation[50];
		char pidToFind[5];
		char pState[MAX_LENGTH];

		while(tmpNode != NULL){

			//printf("stato %d - %d\n", tmpNode->pid, tmpNode->pgid);


			if(shellPID != tmpNode->pgid && kill(tmpNode->pid, 0) == -1){
				printf("\nbackground process %d died", tmpNode->pid);
				printf("\nminishell:->");
				fflush(stdout);

				deleteFromList(tmpNode->pid);

			}else if(shellPID != tmpNode->pgid && kill(tmpNode->pid, 0) > -1){

				strcpy(fileLocation, "/proc/");
				sprintf(pidToFind, "%d", tmpNode->pid);
				strcat(fileLocation, pidToFind );
				strcat(fileLocation, "/stat" );

				processesState = fopen(fileLocation, "r");

				fscanf(processesState, "%s", pState);
				fscanf(processesState, "%s", pState);
				fscanf(processesState, "%s", pState);


				if(strcmp(pState, "Z") == 0){
					printf("\nbackground %d %s - died", tmpNode->pid, tmpNode->usedCommand);
					printf("\nminishell:->");
					fflush(stdout);

					deleteFromList(tmpNode->pid);
				}

				fclose(processesState);

			}


			tmpNode = tmpNode->next;


		}

}


struct backgrNode * findProcess(int procID){

	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;


	while(tmpNode != NULL){

		if(tmpNode->pid == procID){
			return tmpNode;
		}

		tmpNode = tmpNode->next;
	}



	return NULL;
}

void deleteAllList(){

}
