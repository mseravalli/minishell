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
	printf("\nminishell:->");
	fflush(stdout);

	signal(SIGINT, &catch_interrupt);
}

void catch_stop(int sig_num){
    /*
     *  re-set the signal handler again to catch_int, for next time
     *  when Ctrl - Z is pressed the shell should simply go to the next line
     */
	printf("\nminishell:->");
	fflush(stdout);
	signal(SIGTSTP, &catch_stop);
}

void catch_chld(int sig_num){
	notifyDeath();
	signal(SIGCHLD, &catch_chld);
}

void catch_ttin(int sig_num){

	printf("a background process is trying to read from the terminal\n");

	signal(SIGTTIN, &catch_ttin);
}
