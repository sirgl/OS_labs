#pragma once

#include <semaphore.h>

typedef struct DetailType {
	char *name;
	sem_t produced;
} DetailType;


typedef struct ArgumentsComposeFromComponents {
	DetailType **comp;
	int componentsCount;
	DetailType *result;
} ArgumentsComposeFromComponents;

typedef struct ArgumentsProduce {
	DetailType *detail;
	unsigned int delay;
};

DetailType *initDetail(char *name);

void composeFromComponents(ArgumentsComposeFromComponents *arguments);

void produce(ArgumentsProduce* arguments);




