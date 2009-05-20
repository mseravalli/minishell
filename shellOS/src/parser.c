#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* parseString(char *CmdBuf){

	char *element;
	char *argv[64];
	char  *sep = " \t\n";

	//char CmdBuf[] = "ls -l";

	  printf ("\nComplete input: \n%s\nBroken input:\n",CmdBuf);
	  int counter = 0;
	  element = strtok (CmdBuf,sep);
	  while (element != NULL)
	  {
	    argv[counter] = element;
	    printf ("Argument n %d:\t%s\n", counter, argv[counter]);
	    counter++;
	    argv[counter]=NULL;
	    element = strtok(NULL, sep);
	  }
	  printf ("Argument n %d:\t%s\n", counter, argv[counter]);


	  return argv;
}

