#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#include "queue.h"

#define MAX_NAMEPATH 255
#define MAX_AMOUNT_OF_SLAVES 5;
#define MAX_AMOUNT_OF_PIPES 10;


void makeFileQueue(char * path, queueADT queue);

void createProcesses();

#endif
