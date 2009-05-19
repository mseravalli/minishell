#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>



/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
    /* re-set the signal handler again to catch_int, for next time */
    signal(SIGINT, catch_int);
    printf("\nYou have pressed Ctrl-C\n");

}









int main(int argc, char *argv[]) {
	char cmd[80];
	int statval = 1;


	/* set the INT (Ctrl-C) signal handler to 'catch_int' */
	signal(SIGINT, catch_int);



	//infinite loop

	while (1) {
		printf("baby:");

		scanf ("%s",cmd);

		if (strcmp("exit",cmd) == 0) {
			printf("bye!!\n");
			exit(0);
		}


		if (fork()==0) {
			execlp(cmd, NULL, NULL);
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
