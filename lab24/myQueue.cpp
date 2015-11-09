#include "myQueue.h"
#include <stdlib.h>
#include <string.h>

void mymsginit(Queue *queue) {
	queue->first = NULL;
	queue->last = NULL;
	queue->size = 0;
	queue->dropped = 0;
	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->notFull, NULL);
	pthread_cond_init(&queue->notEmpty, NULL);
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

	pthread_mutex_lock(&queue->lock);
	while(queue->size >= MAX_QUEUE_SIZE) {
		pthread_cond_wait(&queue->notFull, &queue->lock);
		if(queue->dropped) {
			pthread_mutex_unlock(&queue->lock);
			return 0;
		}
	}
	if (queue->last == NULL) {
		queue->last = entry;
		queue->first = entry;
		pthread_cond_signal(&queue->notEmpty);
	} else {
		queue->last->next = entry;
		queue->last = entry;
	}
	queue->size++;
	pthread_mutex_unlock(&queue->lock);
	return written;
}

int mymsgget(Queue * queue, char * buf, size_t bufsize){
	pthread_mutex_lock(&queue->lock);
	while(queue->size == 0) {
		pthread_cond_wait(&queue->notEmpty, &queue->lock);
		if(queue->dropped) {
			pthread_mutex_unlock(&queue->lock);
			return 0;
		}
	}
	QueueEntry* tmp;
	if(1 == queue->size) {
		tmp = queue->last;
		queue->last = NULL;
		queue->first = NULL;
	} else {
		tmp = queue->first;
		queue->first = queue->first->next;
		pthread_cond_signal(&queue->notFull);
	}
	queue->size--;
	pthread_mutex_unlock(&queue->lock);

	size_t initialLength = strlen(tmp->message);
	size_t length = initialLength < bufsize - 1 ? initialLength : bufsize - 1;
	strncpy(buf, tmp->message, length);
	buf[length] = 0;
	free(tmp);
	return (int) length;
}

void mymsgdrop(Queue *queue){
	pthread_mutex_lock(&queue->lock);
	queue->dropped = true;
	pthread_cond_signal(&queue->notFull);
	pthread_cond_signal(&queue->notEmpty);
	pthread_mutex_unlock(&queue->lock);
}

void mymsgdestroy(Queue * queue) {
	pthread_mutex_destroy(&queue->lock);
	pthread_cond_destroy(&queue->notFull);
	pthread_cond_destroy(&queue->notEmpty);
	QueueEntry* current = queue->first;
	QueueEntry* tmp;
	while(NULL != current) {
		free(current->message);
		tmp = current;
		current = current->next;
		free(tmp);
	}
}


