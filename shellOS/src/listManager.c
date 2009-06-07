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
/*
	int i = 0;
	for(i = 0; i < MAX_LENGTH/2; i++){
		if(bckgrdList[i].pid != 0)
			printf("%d - %s\n", bckgrdList[i].pid, bckgrdList[i].usedCommand);
			fflush(stdout);
	}



*/

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
	char buf[5];
	char pidToFind[5];
	int isFound = 0;
	char pState[1];

	FILE * processesList;


	processesList =fopen(".ps", "r");

	while(tmpNode != NULL){
		freopen(".ps", "r", processesList);
		isFound = 0;

		sprintf(pidToFind, "%d", tmpNode->pid);

		while(!feof (processesList)){
			fscanf(processesList, "%s", buf);

			if (strcmp(buf, pidToFind) == 0){
				isFound = 1;
				fscanf(processesList, "%s", pState);
				break;
			}


		}

		if(isFound == 1)
			printf("%d - %s \t%s \t stdin: %s \t stdout: %s\n", tmpNode->pid, tmpNode->usedCommand, pState, tmpNode->inResource,  tmpNode->outResource);




		tmpNode = tmpNode->next;
	}


	fclose(processesList);


	/*
	statval = 1;
		childpid = fork();
		if (childpid == 0) {
			fclose (stdout);
			stdout = fopen(".ps1", "w");
			char buf[5];
			sprintf(buf, "%d", shellPID);



			execlp("grep", "grep", "-w", buf, ".ps", NULL);

			exit(1);
		}

		tcsetpgrp(STDIN_FILENO,childpid);

		while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
			usleep(20000);
		}

		tcsetpgrp(STDIN_FILENO,getpid());
		*/


}
