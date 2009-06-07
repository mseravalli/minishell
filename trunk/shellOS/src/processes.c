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

		addToList(childpid, cmd[0], source, destination);

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

		addToList(childpid, cmd[0], source, destination);
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


void put_into_foreground(int pid,int statval){

	kill(pid, SIGCONT);

	int group = getpgid(pid);


	if(group != getpgid(shellPID)){


		setpgid(pid,shellPID);
		tcsetpgrp(STDIN_FILENO,pid);
	}

	waitpid(pid, NULL,WUNTRACED);


	tcsetpgrp(STDIN_FILENO,shellPID);

}
void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);
	//deleteFromList(pid);
}



