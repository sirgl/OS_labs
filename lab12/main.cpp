#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFFER_SIZE = 256;
pthread_mutex_t mutex;
pthread_cond_t accessAvailable;

void printError(int error);

void*childFunction(void *parameter) {
	char* threadName = (char *) parameter;

	pthread_mutex_lock(&mutex);
	for(int i = 0; i < 10; i++) {
		pthread_cond_signal(&accessAvailable);
		printf("%s %d\n", threadName, i);
		pthread_cond_wait(&accessAvailable, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&accessAvailable);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	pthread_t child;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);

	int error;
	error = pthread_mutexattr_settype(&attr, NULL);
	if (error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	error = pthread_mutex_init(&mutex, &attr);
	if (error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	error = pthread_cond_init(&accessAvailable, NULL);
	if (error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	error = pthread_create(&child, NULL, childFunction, (void *) "Child");
	if (error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	childFunction((void *) "Parent");

	pthread_join(child, NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&accessAvailable);
	return(EXIT_SUCCESS);
}

void printError(int error) {
	char buffer[BUFFER_SIZE];
	strerror_r(error, buffer, sizeof buffer);
	fprintf(stderr, "%s\n", buffer);
}