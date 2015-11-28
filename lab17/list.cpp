#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <functional>
#include "list.h"

const int BUFFER_SIZE = 256;

void printError(int error);

List *createList() {
	List *list = (List *) malloc(sizeof(List));
	if (NULL == list) {
		return NULL;
	}
	list->size = 0;
	list->interrupted = 0;
	pthread_mutex_init(&list->head_mutex, NULL);
	return list;
}

ListEntry *createListEntry(char *string) {
	ListEntry *listEntry = (ListEntry *) malloc(sizeof(ListEntry));
	if (NULL == listEntry) {
		return NULL;
	}
	listEntry->string = string;
	listEntry->next = NULL;
	pthread_mutex_init(&listEntry->mutex, NULL);
	return listEntry;
}

void addEntryToList(List *list, ListEntry *entry) {
	pthread_mutex_lock(&list->head_mutex);
	ListEntry *head = list->head;
	if (NULL != head) {
		pthread_mutex_lock(&head->mutex);
	}
	list->head = entry;
	pthread_mutex_unlock(&list->head_mutex);
	entry->next = head;
	if (NULL != head) {
		pthread_mutex_unlock(&head->mutex);
	}
}

void sort(List *list) {
	int sorted = 0;
	ListEntry *i;
	while (!sorted) {
		sorted = 1;
		pthread_mutex_lock(&list->head_mutex);
		i = list->head;
		pthread_mutex_unlock(&list->head_mutex);
		if (NULL == i) {
			break;
		}
		ListEntry *j = i->next;
		while (NULL != j) {
			pthread_mutex_lock(&i->mutex);
			pthread_mutex_lock(&j->mutex);
//			sleep(1); // lab18
			if (0 < strcmp(i->string, j->string)) {
				sorted = 0;
				char *tmp = i->string;
				i->string = j->string;
				j->string = tmp;
			}
			pthread_mutex_unlock(&j->mutex);
			pthread_mutex_unlock(&i->mutex);
			i = i->next;
			j = j->next;
		}
	}
}

void printList(List *list) {
	if (NULL == list->head) {
		return;
	}
	ListEntry *current = list->head;
	pthread_mutex_lock(&current->mutex);
	pthread_mutex_t *previous = &current->mutex;

	while (NULL != current) {
		printf("%s ->", current->string);

		current = current->next;
		if (current == NULL) {
			break;
		}
		pthread_mutex_lock(&current->mutex);
		pthread_mutex_unlock(previous);
		previous = &current->mutex;
	}
	pthread_mutex_unlock(previous);
	printf("\n");
}

void freeList(List *list) {
	ListEntry *entry = list->head;
	ListEntry *toDelete;
	while (NULL != entry) {
		toDelete = entry;
		entry = entry->next;
		pthread_mutex_destroy(&toDelete->mutex);
		free(toDelete);
	}
	list->interrupted = 1;
}