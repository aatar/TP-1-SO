#include <stdlib.h>
#include <stdio.h>
#include "testLib.h"
#include "queueOfFiles.h"

queueADT createQueue(int maxSize)
{
	
	queueADT queue = (queueADT) malloc(sizeof(queueCDT));
	queue->first = (nodeADT) NULL;
	queue->last = (nodeADT) NULL;
	queue->currentSize = 0;
	queue->maxSize = maxSize;
	
	return queue;
}

void enqueueFile(char * fileName, queueADT queue)
{
	
	if(queue->currentSize == queue->maxSize) {
		fail("The queue is full, cannot enqueue file\n");
		return;
	}

	nodeADT newNode = (nodeADT) malloc(sizeof(nodeCDT));

	newNode->fileName = fileName;
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

char * dequeueFile(queueADT queue)
{
	if(isEmpty(queue)){
		fail("The queue is empty, cannot dequeue file\n");
		return NULL;
	}

	char * fileName = queue->first->fileName;
	queue->first = queue->first->next;
	queue->currentSize --;

	return fileName;
}

int isEmpty(queueADT queue)
{
	return (queue->currentSize == 0) ? 1 : 0;
}

int getQueueCurrentSize(queueADT queue)
{
	return queue->currentSize;
}