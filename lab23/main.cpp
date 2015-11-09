#include "myQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int BUFFER_SIZE = 256;

static const int PRODUCER_COUNT = 4;

void printError(int error);

typedef struct Argument {
	char *name;
	Queue *queue;
} Argument;

void *producerFunction(void *parameter) {
	Queue *queue = ((Argument *) parameter)->queue;
	char *name = ((Argument *) parameter)->name;
	char buffer[50];
	for (int i = 0; i < 100000; ++i) {
		sleep(0);
		sprintf(buffer, "%s_%d", name, i);
		mymsgput(queue, buffer);
	}
}

void *consumerFunction(void *parameter) {
	Queue *queue = ((Argument *) parameter)->queue;
	char *name = ((Argument *) parameter)->name;
	for (; ;) {
		char buffer[50];
		mymsgget(queue, buffer, 50);
		fprintf(stdout, "%s: %s\n", name, buffer);
	}
}

int main(int argc, char **argv) {
	Queue queue;
	mymsginit(&queue);
	pthread_t consumers[2];
	pthread_t producers[PRODUCER_COUNT];

	Argument argumentsConsumer[2] = {{(char *) "consumer1", &queue},
	                                 {(char *) "consumer2", &queue}};
	int error = 0;
	for (int i = 0; i < 2; ++i) {
		error = pthread_create(&consumers[i], NULL, consumerFunction, (void *) (&argumentsConsumer[i]));
		if (error) {
			printError(error);
			return EXIT_FAILURE;
		}
	}

	Argument argumentsProducer[PRODUCER_COUNT] = {{(char *) "producer1", &queue},
	                                 {(char *) "producer2", &queue},
	                                 {(char *) "producer3", &queue},
	                                 {(char *) "producer4", &queue}};
	for (int i = 0; i < PRODUCER_COUNT; ++i) {
		error = pthread_create(&producers[i], NULL, (void *(*)(void *)) producerFunction,
		                       (void *) (&argumentsProducer[i]));
		if (error) {
			printError(error);
			return EXIT_FAILURE;
		}
	}

	pthread_exit(EXIT_SUCCESS);
}

void printError(int error) {
	char buffer[BUFFER_SIZE];
	strerror_r(error, buffer, sizeof buffer);
	fprintf(stderr, "%s\n", buffer);
}