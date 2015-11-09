#include "myQueue.h"
#include <stdlib.h>
#include <string.h>

void mymsginit(Queue *queue) {
	queue->first = NULL;
	queue->last = NULL;
	queue->size = 0;
	queue->dropped = 0;
	sem_init(&queue->placesFree, 0, MAX_QUEUE_SIZE);
	sem_init(&queue->placesOccupied, 0, 0);
	sem_init(&queue->queueSem, 0, 1);
}

QueueEntry *getQueueEntry(char *msg, int *written) {
	size_t initialLength = strlen(msg);
	size_t length = initialLength < MAX_MESSAGE_LENGTH ? initialLength : MAX_MESSAGE_LENGTH;
	char *destMsg = (char *) malloc(length + sizeof(char));
	if (NULL == destMsg) {
		return NULL;
	}
	strncpy(destMsg, msg, length);
	destMsg[length] = 0;
	QueueEntry *entry = (QueueEntry *) malloc(sizeof(QueueEntry));
	if (NULL == entry) {
		free(destMsg);
		return NULL;
	}
	entry->message = destMsg;
	entry->next = NULL;
	*written = (int) length;
	return entry;
}

int mymsgput(Queue *queue, char *msg) {
	int written = 0;
	QueueEntry *entry = getQueueEntry(msg, &written);
	if (NULL == entry) {
		return -1;
	}
	sem_wait(&queue->placesFree);
	sem_wait(&queue->queueSem);
	if(queue->dropped) {
		sem_post(&queue->placesFree);
		sem_post(&queue->queueSem);
		return 0;
	}
	queue->placesOccupied;
	if (queue->last == NULL) {
		queue->last = entry;
		queue->first = entry;
	} else {
		queue->last->next = entry;
		queue->last = entry;
	}
	queue->size++;
	sem_post(&queue->queueSem);
	sem_post(&queue->placesOccupied);
}

int mymsgget(Queue * queue, char * buf, size_t bufsize){
	sem_wait(&queue->placesOccupied);
	sem_wait(&queue->queueSem);
	if(queue->dropped) {
		sem_post(&queue->placesOccupied);
		sem_post(&queue->queueSem);
		return 0;
	}
	QueueEntry* tmp;
	if(1 == queue->size) {
		tmp = queue->last;
		queue->last = NULL;
		queue->first = NULL;
	} else {
		tmp = queue->first;
		queue->first = queue->first->next;
	}
	queue->size--;
	sem_post(&queue->queueSem);
	sem_post(&queue->placesFree);


	size_t initialLength = strlen(tmp->message);
	size_t length = initialLength < bufsize - 1 ? initialLength : bufsize - 1;
	strncpy(buf, tmp->message, length);
	buf[length] = 0;
	free(tmp);
	return (int) length;
}

void mymsgdrop(Queue *queue){
	sem_wait(&queue->queueSem);
	queue->dropped = true;
	sem_post(&queue->placesFree);
	sem_post(&queue->placesOccupied);
	sem_post(&queue->queueSem);
}

void mymsgdestroy(Queue * queue) {
	sem_destroy(&queue->queueSem);
	sem_destroy(&queue->placesFree);
	sem_destroy(&queue->placesOccupied);
	QueueEntry* current = queue->first;
	QueueEntry* tmp;
	while(NULL != current) {
		free(current->message);
		tmp = current;
		current = current->next;
		free(tmp);
	}
}


