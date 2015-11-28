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

void composeFromComponents(ArgumentsComposeFromComponents *arguments) {
	DetailType **components = arguments->comp;
	DetailType *result = arguments->result;
	int componentsCount = arguments->componentsCount;
	for(;;) {
		for (int i = 0; i < componentsCount; ++i) {
			sem_wait(&components[i]->produced);
		}
		sem_post(&result->produced);
		printf("Produced %s\n", result->name);
	}
}