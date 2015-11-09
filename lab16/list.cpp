#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

const int BUFFER_SIZE = 256;

void printError(int error);

List *createList() {
	List *list = (List *) malloc(sizeof(List));
	if(NULL == list) {
		return NULL;
	}
	list->head = NULL;
	list->size = 0;
	int error = pthread_mutex_init(&(list->mutex), NULL);
	if(error) {
		free(list);
		printError(error);
		return NULL;
	}
	list->interrupted = 0;
	return list;
}

ListEntry *createListEntry(char* string) {
	ListEntry *listEntry = (ListEntry *) malloc(sizeof(ListEntry));
	if(NULL == listEntry) {
		return NULL;
	}
	listEntry->string = string;
	listEntry->next = NULL;
	return listEntry;
}

void addEntryToList(List *list, ListEntry *entry) {
	pthread_mutex_lock(&list->mutex);

	ListEntry *head = list->head;
	list->head = entry;
	entry->next = head;
	list->size++;

	pthread_mutex_unlock(&list->mutex);
}

void sort(List* list) {
	pthread_mutex_lock(&list->mutex);

	if (NULL == list->head) {
		return;
	}
	for (ListEntry* i = list->head; NULL != i; i = i->next) {
		for (ListEntry* j = i->next; NULL != j; j = j->next) {
			if (0 < strcmp(i->string, j->string)) {
				char* tmp = i->string;
				i->string = j->string;
				j->string = tmp;
			}
		}
	}

	pthread_mutex_unlock(&list->mutex);
}

void printList(List* list) {
	pthread_mutex_lock(&list->mutex);

	printf("size = %d\n", list->size);
	ListEntry *current = list->head;
	while (NULL != current) {
		printf("%s -> ", current->string);
		current = current->next;
	}
	printf("\n");

	pthread_mutex_unlock(&list->mutex);
}

void freeList(List* list) {
	pthread_mutex_t mutex = list->mutex;
	pthread_mutex_lock(&list->mutex);

	ListEntry* entry = list->head;
	ListEntry* toDelete;
	while (NULL != entry) {
		toDelete = entry;
		entry = entry->next;
		free(toDelete);
	}
	list->interrupted = 1;

	pthread_mutex_unlock(&list->mutex);

}