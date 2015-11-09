#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

static const int MAX_STRING_LENGTH = 80;

const int BUFFER_SIZE = 256;

void* childFunction(void* parameter) {
	List* list = (List *) parameter;
	while(!list->interrupted) {
		sleep(5);
		sort(list);
		fprintf(stdout, "Sorted\n");
	}
	pthread_rwlock_destroy(&list->lock);
	free(list);
}

void printError(int error) {
	char buffer[BUFFER_SIZE];
	strerror_r(error, buffer, sizeof buffer);
	fprintf(stderr, "%s\n", buffer);
}

int main(int argc, char **argv) {
	List* list = createList();
	if(NULL == list) {
		fprintf(stderr, "No memory");
		return EXIT_FAILURE;
	}

	char buffer[MAX_STRING_LENGTH + 1];

	pthread_t t;
	int error = pthread_create(&t, NULL, childFunction, list);
	if(error) {
		printError(error);
		return EXIT_FAILURE;
	}

	ssize_t readSize;
	for(;;) {
		readSize = read(0, buffer, MAX_STRING_LENGTH - 1);
		if(0 == readSize) {
			break;
		}
		buffer[readSize] = 0;
		if(0 == strcmp(buffer, "end\n")){
			break;
		}
		if(0 == strcmp(buffer, "\n")) {
			printList(list);
			continue;
		}
		char* entryString = (char *) malloc((readSize + 1) * sizeof(char));
		if(NULL == entryString) {
			fprintf(stderr, "No memory");
			freeList(list);
			pthread_exit((void *) EXIT_FAILURE);
		}
		strcpy(entryString, buffer);

		ListEntry *entry = createListEntry(entryString);
		if(NULL == entry) {
			fprintf(stderr, "No memory");
			freeList(list);
			pthread_exit((void *) EXIT_FAILURE);
		}
		addEntryToList(list, entry);
	}

	pthread_join(t, NULL);
	freeList(list);
	pthread_exit((void *) EXIT_FAILURE);

}