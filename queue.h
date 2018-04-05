#ifndef QUEUE_H
#define QUEUE_H

typedef struct nodeCDT * nodeADT;

typedef struct nodeCDT
{
	const char* data;
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
void enqueue(const char * data, queueADT queue);
const char * dequeue(queueADT queue);
int isEmpty(queueADT queue);
int getQueueCurrentSize(queueADT queue);

#endif
