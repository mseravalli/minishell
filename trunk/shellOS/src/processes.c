#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void run_foreground(char *cmd[], char *argv[], int statval){
	pid_t childpid = fork();
	if (childpid ==0) {

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);

	//tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	//tcsetpgrp(STDIN_FILENO,getpid());

	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
		} else {
			//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}

void run_background(char *cmd[], char *argv[], int statval){
	pid_t childpid = fork();
	if (childpid==0) {

		addToList(getpid(), cmd[0]);
		printList();

		fflush(stdout);
		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);
	tcsetpgrp(STDIN_FILENO,getpid());

}


void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);

}

