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
char SHELL_LOCATION[20];

struct backgrNode{

	int pid;
	char usedCommand[MAX_LENGTH];
	struct backgrNode* next;

};


//struct backgrNode bckgrdList[MAX_LENGTH/2];
struct backgrNode *bckgrdList;

void catch_interrupt();
void catch_stop();
void catch_chld();

void parseString();

void run_background();
void run_foreground();
void put_into_foreground();
void kill_background();

void addToList();
void deleteFromList();
void printList();

void redirectOut();
