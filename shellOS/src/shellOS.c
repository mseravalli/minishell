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
	printf("\nmarco@laptop:->");
	fflush(stdout);
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



int main(int argc, char *argv[]) {

	signal(SIGINT, catch_interrupt);
	signal(SIGTSTP, catch_stop);

	int statval = 1;
	char cmd[MAX_LENGTH];
	char *values[MAX_LENGTH/2];
	int size = 0;


	while (1) {
		printf("marco@laptop:->");
		fflush(stdout);

		fgets(cmd, MAX_LENGTH, stdin);

		parseString(&cmd, values, &size);

		if (strcmp("\n",values[0]) == 0) {
			continue;
		}


		if (strcmp("exit",values[0]) == 0) {
			printf("Bye!!\n");
			exit(0);
		}

		if (strcmp("kill",values[0]) == 0) {
			kill_background(atoi(values[1]));
			continue;
		}


		if(*values[size-1] == '&'){

			values[size-1] = NULL;

			printf("process launched in background\n");
			run_background(values, argv, statval);
		}
		else{
			printf("process launched in foreground\n");
			run_foreground(values, argv, statval);
		}
	}

	exit(0);
}
