#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"


void catch_interrupt(int sig_num){
    /*
     *  re-set the signal handler again to catch_int, for next time
     *  when Ctrl - C is pressed the shell should simply go to the next line
     */
	printf("\n");
    signal(SIGINT, catch_interrupt);
}

void catch_stop(int sig_num){
    /*
     *  re-set the signal handler again to catch_int, for next time
     *  when Ctrl - Z is pressed the shell should simply go to the next line
     */
	printf("you're trying to stop something\n");
    signal(SIGTSTP, catch_stop);
}

void run_foreground(char *cmd[], char *argv[], int statval){

	printf("%s\n", cmd[0]);

	if (fork()==0) {

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

void run_background(char *cmd, char *argv[], int statval){

	if (fork()==0) {
		setsid();
		execlp(cmd, cmd, NULL);
		fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd, strerror(errno));
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



int main(int argc, char *argv[]) {

	signal(SIGINT, catch_interrupt);
	signal(SIGTSTP, catch_stop);

	int statval = 1;
	char cmd[128];
	char *values[64];
	int size = 0;



	while (1) {
		printf("marco@laptop:->");

		gets(cmd);
		parseString(&cmd, values, &size);



		if (strcmp("exit",cmd) == 0) {
			printf("Bye!!\n");
			exit(0);
		}

		if(*values[size-1] == '&'){
			printf("process launched in background\n");
			run_background(values[0], argv, statval);
		}
		else{
			printf("process launched in foreground\n");
			run_foreground(values, argv, statval);
		}
	}

	exit(0);
}
