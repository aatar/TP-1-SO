#ifndef QUEUE_OF_FILES_H
#define QUEUE_OF_FILES_H

typedef struct nodeCDT * nodeADT;

typedef struct nodeCDT
{
	char* fileName;
	nodeADT next;
}nodeCDT;

typedef struct queueCDT * queueADT;

typedef struct queueCDT
{
	nodeADT first;
	nodeADT last;
	int currentSize;
}queueCDT;

queueADT createQueue();
void enqueueFile(char * fileName, queueADT queue);
char * dequeueFile(queueADT queue);
int isEmpty(queueADT queue);
int getQueueCurrentSize(queueADT queue);

#endif
