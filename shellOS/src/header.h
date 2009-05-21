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
	struct backgrdNode* next;

}backgrNode;



void parseString();
void run_background();
void run_foreground();
void kill_background();
void printList();
