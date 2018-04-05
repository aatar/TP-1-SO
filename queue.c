#include <stdlib.h>
#include <stdio.h>
#include "testLib.h"
#include "queue.h"

queueADT createQueue()
{
	queueADT queue = (queueADT) malloc(sizeof(queueCDT));
	queue->first = (nodeADT) NULL;
	queue->last = (nodeADT) NULL;
	queue->currentSize = 0;
	return queue;
}

void enqueue(char * data, queueADT queue)
{
	nodeADT newNode = (nodeADT) malloc(sizeof(nodeCDT));

	newNode->data = data;
	newNode->next = NULL;

	if(isEmpty(queue))
	{
		queue->first = newNode;

	}
	else
	{
		queue->last->next = newNode;
	}

	queue->currentSize ++;
	queue->last = newNode;
}

char * dequeue(queueADT queue)
{
	if(isEmpty(queue)){
		fail("The queue is empty, can't dequeue file\n");
		return NULL;
	}

	char * data = queue->first->data;
	queue->first = queue->first->next;
	queue->currentSize --;

	return data;
}

int isEmpty(queueADT queue)
{
	return (queue->currentSize == 0) ? 1 : 0;
}

int getQueueCurrentSize(queueADT queue)
{
	return queue->currentSize;
}
