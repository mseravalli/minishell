#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseString(char *CmdBuf, char *argv[], int *size){

	char *element;

	char  *sep = " \t\n";

	  int counter = 0;
	  element = strtok (CmdBuf,sep);
	  while (element != NULL)
	  {
	    argv[counter] = element;
	    counter++;
	    argv[counter]=NULL;
	    element = strtok(NULL, sep);
	  }

	  *size = counter;
}

