#pragma once

#include <pthread.h>

typedef struct ListEntry {
	char* string;
	ListEntry* next;
} ListEntry;


typedef struct List {
	pthread_mutex_t mutex;
	ListEntry* head;
	int size;
	int interrupted;
} List;

ListEntry * createListEntry(char* string);

List * createList();

void addEntryToList(List *list, ListEntry *entry);

void sort(List* list);

void printList(List* list);

void freeList(List* list);