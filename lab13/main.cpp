#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

const int BUFFER_SIZE = 256;
sem_t semaphore[2];

void printError(int error);

void *childFunction(void *parameter) {
	for(int i = 0; i < 10; i++) {
		sem_wait(&semaphore[1]);
		printf("Child %d\n", i);
		sem_post(&semaphore[0]);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t child;

	int error;
	error = sem_init(&semaphore[0], 0, 0);
	if(error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	error = sem_init(&semaphore[1], 0, 1);
	if(error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	error = pthread_create(&child, NULL, childFunction, NULL);
	if (error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < 10; i++) {
		sem_wait(&semaphore[0]);
		printf("Parent %d\n", i);
		sem_post(&semaphore[1]);
	}

	pthread_join(child, NULL);
	sem_destroy(&semaphore[0]);
	sem_destroy(&semaphore[1]);
	return (EXIT_SUCCESS);
}

void printError(int error) {
	char buffer[BUFFER_SIZE];
	strerror_r(error, buffer, sizeof buffer);
	fprintf(stderr, "%s\n", buffer);
}