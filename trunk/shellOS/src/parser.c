#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseString(char *CmdBuf, char *argv[], int *size){

	char *element;

	char  *sep = " \t\n";

	  //printf ("\nComplete input: \n%s\nBroken input:\n",CmdBuf);

	  int counter = 0;
	  element = strtok (CmdBuf,sep);
	  while (element != NULL)
	  {
	    argv[counter] = element;
	    //printf ("Argument n %d:\t%s\n", counter, argv[counter]);
	    counter++;
	    argv[counter]=NULL;
	    element = strtok(NULL, sep);
	  }
	  //printf ("Argument n %d:\t%s\n", counter, argv[counter]);

	  *size = counter;
}

