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

	signal(SIGINT, &catch_interrupt);
}

void catch_stop(int sig_num){
    /*
     *  re-set the signal handler again to catch_int, for next time
     *  when Ctrl - Z is pressed the shell should simply go to the next line
     */
	printf("\nmarco@laptop:->");
	fflush(stdout);
	signal(SIGTSTP, &catch_stop);
}

void catch_chld(int sig_num){

}


int main(int argc, char *argv[]) {

	strcpy(SHELL_LOCATION, "\/dev\/tty");

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


	char redirectOutput[] = "/dev/tty";
	char redirectInput[] = "/dev/tty";
	int readOffset = 0;
	int readPosition = 0;


	shellPID = getpid();

	while (1) {
		statval2 = 1;
		statval = 1;
		printf("marco@laptop:->");
		fflush(stdout);


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
			if(values[1] == NULL)
				continue;
			kill_background(atoi(values[1]));
			continue;
		}

		if(strcmp("fg",values[0]) == 0){
			printf("putting in foreground %d\n", atoi(values[1]));
			fflush(stdout);
			put_into_foreground(atoi(values[1]),statval);
			continue;
		}

		if (strcmp("jobs",values[0]) == 0) {

			printList();

			continue;
		}


		/*
		 * redirection of input
		 */
		if (strcmp("in",values[0]) == 0) {
			if(values[1] != NULL && values[2] != NULL){
				int i = 0;

				strcpy(redirectInput, values[1]);
				printf("input redirect to %s\n", values[1]);

				for(i = 0; i < (size - 2) ;i++){
					strcpy(values[i], values[i+2]);
				}

				values[size - 1] = NULL;
				values[size - 2] = NULL;

				size = size - 2;

			} else  {
				continue;
			}

		}


		/*
		 * redirection of output
		 */

		if (strcmp("out",values[0]) == 0) {
			if(values[1] != NULL && values[2] != NULL){
				int i = 0;

				strcpy(redirectOutput, values[1]);
				printf("output redirect to %s\n", values[1]);

				for(i = 0; i < (size - 2) ;i++){
					strcpy(values[i], values[i+2]);
				}

				values[size - 1] = NULL;
				values[size - 2] = NULL;

				size = size - 2;

			} else  {
				continue;
			}

		}




		if(*values[size-1] == '&'){

			values[size-1] = NULL;

			run_background(values, argv, statval, redirectInput, redirectOutput);
		}
		else{

			run_foreground(values, argv, statval, redirectInput, redirectOutput);
		}

		strcpy(redirectOutput, SHELL_LOCATION);
		strcpy(redirectInput, SHELL_LOCATION);

	}

	exit(0);

}
