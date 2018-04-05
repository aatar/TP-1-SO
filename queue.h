#ifndef QUEUE_H
#define QUEUE_H

typedef struct nodeCDT * nodeADT;

typedef struct nodeCDT
{
	char* data;
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
void enqueue(char * data, queueADT queue);
char * dequeue(queueADT queue);
int isEmpty(queueADT queue);
int getQueueCurrentSize(queueADT queue);

#endif
