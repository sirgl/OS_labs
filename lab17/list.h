#pragma once

#include <pthread.h>

typedef struct ListEntry {
	char* string;
	ListEntry* next;
	pthread_mutex_t mutex;
} ListEntry;


typedef struct List {
	ListEntry* head;
	int size;
	int interrupted;
	pthread_mutex_t head_mutex;
} List;

ListEntry * createListEntry(char* string);

List * createList();

void addEntryToList(List *list, ListEntry *entry);

void sort(List* list);

void printList(List* list);

void freeList(List* list);