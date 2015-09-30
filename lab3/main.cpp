#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


void *calc(void *strings) {
	char **strs = (char **) strings;
	int count = (int) (pthread_self() % 4);
	for (int i = 0; i < count; ++i) {
		strs++;
	}
	while(*strs != NULL) {
		printf("%d: %s\n", count, *strs);
		strs++;
	}
	return 0;
}

int main() {
	char* strs[] = {"1", "2", "3", "4", "5", "6", "7", NULL};
	pthread_t threads[4];
	for (int i = 0; i < 4; ++i) {
		pthread_create(&threads[i], NULL, calc, strs);
	}
	for (int i = 0; i < 4; ++i) {
		pthread_join(threads[i], NULL);
	}
	pthread_exit(NULL);
}