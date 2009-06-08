#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void addToList(int procID, char cmd[], char inRes[], char outRes[]){

/*
	int i = 0;
	for(i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid == 0)
			break;
	}

	//printf("inserting at the %dth position\n", i);
	bckgrdList[i].pid = procID;
	strcpy( bckgrdList[i].usedCommand, cmd );
*/

	/*
	char value[MAX_LENGTH];

	strcpy( value, cmd );

*/
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


void printList(){

	int statval = 1;
	pid_t childpid = fork();
	if (childpid == 0) {
		fclose (stdout);
		stdout = fopen(".ps", "w");

		execlp("ps", "ps", "-eo", "pid,state", NULL);

		exit(1);
	}

	tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,getpid());




	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;
	char pidFound[5];
	char pidToFind[5];
	int isFound = 0;
	//char pState[5];

	FILE * processesList;


	processesList =fopen(".ps", "r");

	while(tmpNode != NULL){


		freopen(".ps", "r", processesList);
		isFound = 0;

		sprintf(pidToFind, "%d", tmpNode->pid);

		while(!feof (processesList)){
			fscanf(processesList, "%s", pidFound);

			if (strcmp(pidFound, pidToFind) == 0){
				isFound = 1;
				fscanf(processesList, "%s", tmpNode->pState);
			}


		}

		if(isFound == 1 && strcmp(tmpNode->pState, "Z") != 0){
			printf("%d - %s \t%s \t stdin: %s \t stdout: %s\n", tmpNode->pid, tmpNode->usedCommand, tmpNode->pState, tmpNode->inResource,  tmpNode->outResource);
			fflush(stdout);
		}


		tmpNode = tmpNode->next;
	}


	fclose(processesList);



}



void updateState(){

	int statval = 1;
	pid_t childpid = fork();
	if (childpid == 0) {
		fclose (stdout);
		stdout = fopen(".ps", "w");

		execlp("ps", "ps", "-eo", "pid,state", NULL);
		//fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,getpid());




	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;
	char pidFound[5];
	char pidToFind[5];
	int isFound = 0;
	char pListState[5];

	FILE * processesList;


	processesList =fopen(".ps", "r");

	while(tmpNode != NULL){


		freopen(".ps", "r", processesList);
		isFound = 0;

		sprintf(pidToFind, "%d", tmpNode->pid);

		while(!feof (processesList)){
			fscanf(processesList, "%s", pidFound);

			if (strcmp(pidFound, pidToFind) == 0){
				isFound = 1;
				fscanf(processesList, "%s", pListState);
			}


		}

		if(isFound == 1){

			if(tmpNode->pState == NULL){

				printf("\n%d - %s \t%s \t stdin: %s \t stdout: %s\n", tmpNode->pid, tmpNode->usedCommand, pListState, tmpNode->inResource,  tmpNode->outResource);
				fflush(stdout);

				strcpy(tmpNode->pState, pListState);

			} else if((strcmp(pListState, tmpNode->pState) != 0)){

				if((strcmp(pListState, "T") == 0)){

					printf("\n%d - %s \t%s\n", tmpNode->pid, tmpNode->usedCommand, "waiting for an input");
					fflush(stdout);
				}

				if((strcmp(pListState, "Z") == 0)){

					printf("\n%d - %s \t%s\n", tmpNode->pid, tmpNode->usedCommand, "died");
					fflush(stdout);
				}

				strcpy(tmpNode->pState, pListState);
			}
		}



		tmpNode = tmpNode->next;
	}


	fclose(processesList);



}

