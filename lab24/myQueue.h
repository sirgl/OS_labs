#pragma once

#include <stddef.h>
#include <pthread.h>

typedef struct QueueEntry {
	char* message;
	QueueEntry* next;
} QueueEntry;

typedef struct Queue {
	QueueEntry* first;
	QueueEntry* last;
	size_t size;
	pthread_mutex_t lock;
	pthread_cond_t notEmpty;
	pthread_cond_t notFull;
	int dropped;
} queue;

void mymsginit(Queue *);
void mymsgdrop(Queue *);
void mymsgdestroy(Queue *);
int mymsgput(Queue *, char * msg);
int mymsgget(Queue *, char * buf, size_t bufsize);
QueueEntry* getQueueEntry(char *msg, int *written);

const size_t MAX_MESSAGE_LENGTH = 80;
const size_t MAX_QUEUE_SIZE = 10;