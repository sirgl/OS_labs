#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

static const int MAX_STRING_LENGTH = 80;

void* childFunction(void* parameter) {
	List* list = (List *) parameter;
	while(!list->interrupted) {
		sleep(5);
		sort(list);
	}
	pthread_mutex_destroy(&list->mutex);
	free(list);
}

int main(int argc, char **argv) {
	List* list = createList();
	if(NULL == list) {
		fprintf(stderr, "No memory");
		return EXIT_FAILURE;
	}

	char buffer[MAX_STRING_LENGTH + 1];

	ssize_t readSize;
	pthread_t t;
	pthread_create(&t, NULL, childFunction, list);
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

	freeList(list);
	pthread_exit((void *) EXIT_FAILURE);

}