#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void run_foreground(char *cmd[], char *argv[], int statval, char *destination){
	pid_t childpid = fork();
	if (childpid ==0) {

		signal(SIGINT, SIG_DFL);
		//signal(SIGTSTP, SIG_DFL);
		if (strcmp("/dev/tty",destination) != 0){
			fclose (stdout);
			stdout = fopen(destination, "a");
		};

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);

	tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, 1,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,shellPID);

	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
		} else {
			//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}

void run_background(char *cmd[], char *argv[], int statval, char *destination){
	pid_t childpid = fork();

	if (childpid > 0){
		addToList(childpid, cmd[0]);
	}


	if (childpid==0) {

		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);

		if (strcmp("/dev/tty",destination) != 0){
			fclose (stdout);
			stdout = fopen(destination, "a");
		}

		int i;
		for (i=getdtablesize();i>=0;--i)
			close(i);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);
	tcsetpgrp(STDIN_FILENO,shellPID);
	return;

}


void put_into_foreground(int pid,int statval){
	tcsetpgrp(STDIN_FILENO,pid);

	while(waitpid(pid, 1,WNOHANG | WUNTRACED) == 0){
		printf("waiting");
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,shellPID);
}

void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);
	deleteFromList(pid);
}



