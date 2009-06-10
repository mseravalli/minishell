#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void run_foreground(char *cmd[], char *argv[], int statval, char *source, char *destination){
	pid_t childpid = fork();

	if (childpid ==0) {

		//signal(SIGTSTP, SIG_IGN);

		if (strcmp(SHELL_LOCATION, destination) != 0){
			fclose (stdout);
			stdout = fopen(destination, "a");
		}

		if (strcmp(SHELL_LOCATION, source) != 0){
			fclose (stdin);
			stdin = fopen(source, "r");
		}


		tcsetpgrp(STDIN_FILENO,childpid);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	if(childpid > 0){

		addToList(childpid, shellPID, cmd[0], source, destination);

		setpgid(childpid,shellPID);

		while (waitpid(childpid, NULL ,WNOHANG | WUNTRACED) == 0 ){
			usleep(30000);

		}

		tcsetpgrp(STDIN_FILENO,shellPID);

	}

	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
	} else {
		//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}

void run_background(char *cmd[], char *argv[], int statval, char *source, char *destination){



	pid_t childpid = fork();

	if (childpid > 0){

		addToList(childpid, childpid, cmd[0], source, destination);
	}


	if (childpid==0) {

		setpgid(childpid,childpid);

		/*int i;
		for (i=getdtablesize();i>=0;--i){
			close(i);
		}*/
		fclose (stdout);
		if (strcmp(SHELL_LOCATION, destination) != 0){
			stdout = fopen(destination, "a");
		}

		fclose (stdin);
		if (strcmp(SHELL_LOCATION, source) != 0){
			stdin = fopen(source, "r");
		}




		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	return;

}


int put_into_foreground(int pid){



	int group;

	if(findProcess(pid) != NULL){
		group = findProcess(pid)->pgid;
	} else{
		printf("the jobs was not in the list\n");
		return -1;
	}

	kill(pid, SIGCONT);

	//int group = getpgid(pid);


	if(group != shellPID){

		findProcess(pid)->pgid = shellPID;

		setpgid(pid,shellPID);
		tcsetpgrp(STDIN_FILENO,pid);
	}

	waitpid(pid, NULL,WUNTRACED);


	tcsetpgrp(STDIN_FILENO,shellPID);

	return 0;

}


int kill_background(int pid){

	if(findProcess(pid) != NULL){

		kill(pid, SIGKILL);

		return 0;
	} else{
		printf("the jobs was not in the list\n");
		return -1;
	}

}



