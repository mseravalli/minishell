#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void addToList(int procID, char cmd[]){

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




	struct backgrNode *tmpNode;
	tmpNode = bckgrdList;
	while(tmpNode != NULL){

		printf("%d - %s\n", tmpNode->pid, tmpNode->usedCommand);
		tmpNode = tmpNode->next;

	}*/


	int statval = 1;
	pid_t childpid = fork();
	if (childpid == 0) {
		fclose (stdout);
		stdout = fopen(".ps", "w");

		execlp("ps", "ps", "-eo", "pid,comm,ppid,state,pgid", NULL);
		//fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,getpid());





	statval = 1;
		childpid = fork();
		if (childpid == 0) {

			char buf[5];
			sprintf(buf, "%d", shellPID);



			execlp("grep", "grep", "-w", buf, ".ps", NULL);
			//fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
			exit(1);
		}

		tcsetpgrp(STDIN_FILENO,childpid);

		while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
			usleep(20000);
		}

		tcsetpgrp(STDIN_FILENO,getpid());


}
