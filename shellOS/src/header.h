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


struct backgrNode{

	int pid;
	char *usedCommand;
	struct backgrdNode* next;

};


struct backgrNode bckgrdList[MAX_LENGTH/2];


void parseString();
void run_background();
void run_foreground();
void put_into_foreground();
void kill_background();
void addToList();
void printList();
void redirectOut();