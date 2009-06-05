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
			fclose (stdout);
			stdout = fopen(destination, "a");
		}

		tcsetpgrp(STDIN_FILENO,childpid);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	if(childpid > 0){
		setpgid(childpid,shellPID);




		while (waitpid(childpid, NULL ,WNOHANG | WUNTRACED) == 0 ){
			usleep(30000);
			//printf("%d", waitpid(childpid, NULL ,WNOHANG | WUNTRACED));

		}

		tcsetpgrp(STDIN_FILENO,shellPID);

		/*
		setpgid(childpid,shellPID);
		tcsetpgrp(STDIN_FILENO,childpid);


		waitpid(childpid, NULL,WUNTRACED|WNOHANG);
		//setpgid(shellPID,shellPID);
		tcsetpgrp(STDIN_FILENO,shellPID);
		*/
	}

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
		setpgid(childpid,childpid);
		if (strcmp("/dev/tty",destination) != 0){
			fclose (stdout);
			//fclose(stdin);
			stdout = fopen(destination, "a");
		}

		int i;
		for (i=getdtablesize();i>=0;--i)
			close(i);

		execvp(cmd[0], cmd);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd[0], strerror(errno));
		exit(1);
	}

	//tcsetpgrp(STDIN_FILENO,shellPID);
	return;

}


void put_into_foreground(int pid,int statval){

	kill(pid, SIGCONT);

	int group = getpgid(pid);



	if(group != getpgid(shellPID)){

		//printf("the process did not belong to the pgid of the shell \n");
		//fflush(stdout);

		setpgid(pid,shellPID);
		tcsetpgrp(STDIN_FILENO,pid);
	}

	waitpid(pid, NULL,WUNTRACED);

	//setpgid(shellPID,shellPID);

	tcsetpgrp(STDIN_FILENO,shellPID);

}
void kill_background(int pid){

	printf("%d\n",pid);
	fflush(stdout);

	kill(pid, SIGKILL);
	//deleteFromList(pid);
}



