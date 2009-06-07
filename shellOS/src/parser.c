#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void parseString(char *CmdBuf, char *argv[], int *size){


	//argv is cleaned
	int i;
	for(i = 0; i<MAX_LENGTH/2; i++)
		argv[i] = NULL;

	printf("");
	fflush(stdout);

	char *element;

	char  *sep = " \t\n";



	int counter = 0;
	element = strtok (CmdBuf,sep);
	while (element != NULL){

		argv[counter] = malloc(MAX_LENGTH*sizeof(char));

		strcpy(argv[counter], element);

	    //argv[counter] = element;
	    counter++;
	    argv[counter]=NULL;
	    element = strtok(NULL, sep);
	}

	*size = counter;


}

