/*
 * header.h
 *
 *  Created on: 20-mag-2009
 *      Author: marco
 */

#ifndef HEADER_H_
#define HEADER_H_
#define MAX_LENGTH 1024


#endif /* HEADER_H_ */


int shellPID;
char SHELL_LOCATION[MAX_LENGTH];

struct backgrNode{

	int pid;
	int pgid;
	char usedCommand[MAX_LENGTH];

	char inResource[MAX_LENGTH];
	char outResource[MAX_LENGTH];

	char pState[5];

	struct backgrNode* next;

};


//struct backgrNode bckgrdList[MAX_LENGTH/2];
struct backgrNode *bckgrdList;


void catch_interrupt();
void catch_stop();
void catch_chld();
void catch_ttin();


void parseString(char *CmdBuf, char *argv[], int *size);


void run_foreground(char *cmd[], char *argv[], int statval, char *source, char *destination);
void run_background(char *cmd[], char *argv[], int statval, char *source, char *destination);
void put_into_foreground(int pid);
void kill_background(int pid);


void addToList(int procID,int procGID, char cmd[], char inRes[], char outRes[]);
void deleteFromList(int procID);
void printList();
void notifyDeath();
