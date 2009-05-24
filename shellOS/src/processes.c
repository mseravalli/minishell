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

		if (strcmp("/dev/tty",destination) != 0){
			printf("the output is redirect to %s\n", destination);
			fclose (stdout);
			stdout = fopen(destination, "w");
		}

		printf("%d - %s\n", getpid(), cmd[0]);
		fflush(stdout);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);

	tcsetpgrp(STDIN_FILENO,childpid);

	while(waitpid(childpid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}

	tcsetpgrp(STDIN_FILENO,getpid());

	if (WIFEXITED(statval)) {
		if (WEXITSTATUS(statval))
			fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
		} else {
			//fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
	}

}

void run_background(char *cmd[], char *argv[], int statval){
	pid_t childpid = fork();

	if (childpid > 0){
		addToList(childpid, cmd[0]);
		printList();
		fflush(stdout);
	}


	if (childpid==0) {

		int i;
		for (i=getdtablesize();i>=0;--i)
			close(i);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}
	setpgid(childpid,childpid);
	tcsetpgrp(STDIN_FILENO,getpid());

}


void put_into_foreground(int pid,int statval){
	printf("\n %d \n",pid);
	setpgid(pid,pid);
	tcsetpgrp(STDIN_FILENO,pid);

	while(waitpid(pid, &statval,WNOHANG | WUNTRACED) == 0){
		usleep(20000);
	}
	tcsetpgrp(STDIN_FILENO,getpid());
}

/*need the pid of the current process
void put_into_background(){
	setpgid(getpid(),getpid());
	tcsetpgrp(STDIN_FILENO,getpid());
}*/


void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);
}


