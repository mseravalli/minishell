#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

     /*void sigproc(void);

     void quitproc(void);*/


void sigproc(){
	signal (SIGINT, sigproc);
	printf("You have pressed CTRL-C \n");
}

void quitproc(){
	printf("ctrl-\\ pressed to quit \n");
	exit(0); /* normale status di uscita */
}


main(int argc, char *argv[]){

	signal(SIGINT, sigproc);
	signal(SIGQUIT, quitproc);


	char cmd[80];
	int statval = 1;


          	//infinite loop

	while (1) {
		printf("baby:");

		scanf ("%s",cmd);

		if (strcmp("exit",cmd) == 0) {
			printf("bye!!\n");
			exit(0);
		}


          		//we are in the child

          		int forkResult = fork();


          		if(forkResult == 0) {

          			signal(SIGINT, sigproc);
          			signal(SIGQUIT, quitproc);

          			execlp(cmd, cmd, NULL);
          			fprintf(stderr,"%s: EXEC of %s failed: %s\n", argv[0], cmd, strerror(errno));
          			exit(1);
          		}

          		wait(&statval);

          		if (WIFEXITED(statval)) {
          			if (WEXITSTATUS(statval))
          				fprintf(stderr, "%s: child exited with status %d\n", argv[0], WEXITSTATUS(statval));
          		} else {
          			fprintf(stderr,"%s: child died unexpectedly\n", argv[0]);
          		}

          	}
}
