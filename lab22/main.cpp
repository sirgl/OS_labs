#include "detail_type.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

const int COUNT = 5;

const int BUFFER_SIZE = 256;

void printError(int error) ;

int main(int argc, char **argv) {
	DetailType* details[COUNT];
	details[0] = initDetail((char *) "A");
	details[1] = initDetail((char *) "B");
	details[2] = initDetail((char *) "C");
	details[3] = initDetail((char *) "ABmodule");
	details[4] = initDetail((char *) "widget");

	pthread_t threads[5];
	ArgumentsProduce arguments[3];
	int error;
	for (unsigned int i = 0; i < 3; ++i) {
		arguments[i].detail = details[i];
		arguments[i].delay = i + 1;
		error = pthread_create(&threads[i], NULL, (void *(*)(void *)) produce, &arguments[i]);
		if(error) {
			printError(error);
			exit(EXIT_FAILURE);
		}
	}

	DetailType* detailsArgs1[2] = {details[0], details[1]};
	ArgumentsComposeFromComponents args1 = {detailsArgs1, 2, details[3]};
	error = pthread_create(&threads[3], NULL, (void *(*)(void *)) composeFromComponents, &args1);
	if(error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	DetailType* detailsArgs2[2] = {details[2], details[3]};
	ArgumentsComposeFromComponents args2 = {detailsArgs2, 2, details[4]};
	error = pthread_create(&threads[3], NULL, (void *(*)(void *)) composeFromComponents, &args2);
	if(error) {
		printError(error);
		exit(EXIT_FAILURE);
	}

	pthread_exit(EXIT_SUCCESS);
}

void printError(int error) {
	char buffer[BUFFER_SIZE];
	strerror_r(error, buffer, sizeof buffer);
	fprintf(stderr, "%s\n", buffer);
}