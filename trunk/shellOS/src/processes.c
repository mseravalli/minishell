#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"





void addBackgrdProc(struct backgrNode *list, int procID){

	struct backgrNode *tmpNode;
	tmpNode = malloc(sizeof(struct backgrNode));

	tmpNode->pid = procID;

	if(list != NULL)
		list->next = tmpNode;
	else
		list = tmpNode;

}

void printList(struct backgrNode *list){

	while(list != NULL){

		printf("%d\n", list->pid);
		list = list->next;

	}


}


void run_foreground(char *cmd[], char *argv[], int statval, struct backgrNode *list){

	if (fork()==0) {

		addBackgrdProc(list, getpid());

		printList(list);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	wait(&statval);
	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
		} else {
			//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}

void run_background(char *cmd[], char *argv[], int statval){

	if (fork()==0) {
		setsid();
		printf("%d\n", getpid());
		fflush(stdout);
		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	//the shell should not wait for the end of the process
	//wait(&statval);

	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
	} else {
		//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}


void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);

}

