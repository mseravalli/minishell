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
	printf("\nmarco@laptop:->");
	fflush(stdout);
    signal(SIGTSTP, catch_stop);
}

void catch_chld(int sig_num){

}


int main(int argc, char *argv[]) {

	signal(SIGINT, &catch_interrupt);
	signal(SIGTSTP, &catch_stop);
	signal(SIGCHLD, &catch_chld);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

	int statval = 1;
	int statval2 = 1;
	char cmd[MAX_LENGTH];
	char *values[MAX_LENGTH/2];
	int size = 0;

	FILE *shellOut = freopen("/dev/tty", "w",stdout);

	char destination[] = "/dev/tty";

	int i;
	for(i = 0; i < MAX_LENGTH/2; i++){
		bckgrdList[i].pid = 0;
	}


	addToList(getpid(), "main program");

	while (1) {


		printf("marco@laptop:->");
		fflush(shellOut);


		fgets(cmd, MAX_LENGTH, stdin);

		if ('\n' == cmd[0]) {
			continue;
		}


		parseString(&cmd, values, &size);


		if (strcmp("exit",values[0]) == 0 || strcmp("quit",values[0]) == 0) {
			printf("Bye!!\n");
			exit(0);
		}


		if (strcmp("kill",values[0]) == 0) {
			kill_background(atoi(values[1]));
			continue;
		}

		if(strcmp("fg",values[0]) == 0){
			printf("putting in foreground %d", atoi(values[1]));
			put_into_foreground(atoi(values[1]),statval2);
			continue;
		}

		if (strcmp("jobs",values[0]) == 0) {
			printf("this is the list\n");
			printList();
			fflush(stdout);
			continue;
		}

		if (strcmp("out",values[0]) == 0) {
			if(values[1] != NULL){
				strcpy(destination, values[1]);
				printf("output redirect to %s\n", values[1]);
			}
			continue;
		}


		if (strcmp("restoreout",values[0]) == 0) {
			printf("restoring stdout\n");
			strcpy(destination, "\/dev\/tty");
			continue;
		}


		if(*values[size-1] == '&'){

			values[size-1] = NULL;

			printf("process launched in background\n");
			run_background(values, argv, statval);
		}
		else{
			printf("process launched in foreground\n");

			run_foreground(values, argv, statval, destination);
		}
	}

	exit(0);

}