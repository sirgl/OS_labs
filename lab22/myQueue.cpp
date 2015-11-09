#include "detail_type.h"
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


DetailType *initDetail(char* name) {
	DetailType *detail = (DetailType *) malloc(sizeof(DetailType));
	if(NULL == detail) {
		return NULL;
	}
	sem_init(&detail->produced, 0, 0);
	detail->name = name;
	return detail;
}

void destroyDetail(DetailType* detail) {
	if(NULL != detail) {
		sem_destroy(&detail->produced);
	}
	free(detail);
}

void produce(ArgumentsProduce* arguments) {
	unsigned int delay = arguments->delay;
	DetailType* type = arguments->detail;
	for(;;) {
		sleep(delay);
		sem_post(&type->produced);
		fprintf(stdout, "Produced %s\n", type->name);
	}
}

void produceFromComponents(ArgumentsProduceFromComponents* arguments) {
	DetailType *comp1 = arguments->comp1;
	DetailType *comp2 = arguments->comp2;
	DetailType *result = arguments->result;
	for(;;) {
		sem_wait(&comp1->produced);
		sem_wait(&comp2->produced);
		sem_post(&result->produced);
		printf("Produced %s\n", result->name);
	}
}