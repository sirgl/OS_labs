#pragma once

#include <semaphore.h>

typedef struct DetailType {
	char *name;
	sem_t produced;
} DetailType;


typedef struct ArgumentsProduceFromComponents {
	DetailType *comp1;
	DetailType *comp2;
	DetailType *result;
} ArgumentsProduceFromComponents;

typedef struct ArgumentsProduce {
	DetailType *detail;
	unsigned int delay;
} Arguments;

DetailType *initDetail(char *name);

void produceFromComponents(ArgumentsProduceFromComponents* arguments);

void destroyDetail(DetailType *detail);

void produce(ArgumentsProduce* arguments);




