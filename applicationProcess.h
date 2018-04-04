#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#include "queue.h"

void makeFileQueue(char * path, queueADT queue);
void readSlavePipe(int index);
void answerSlaveRequest(int index);
void applicationProcess();
int thereAreSlavesAlive();
void closeAll();


#endif
