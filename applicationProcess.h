#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#include "queue.h"

#define MAX_NAMEPATH 255
#define MAX_BUFFER_SIZE 290
#define MAX_AMOUNT_OF_SLAVES 1
#define MAX_PIPENAME_LENGTH 30
#define KILL_MESSAGE "KILL"
#define WAITING_MESSAGE "WAITING"

void makeFileQueue(const char * path, queueADT queue);
void readSlavePipe(int index);
void answerSlaveRequest(int index);
void applicationProcess();
int thereAreSlavesAlive();
void closeAll();


void createFile();

#endif
