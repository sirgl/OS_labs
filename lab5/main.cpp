#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void cancellationHandler(void*) {
	printf("interrupted!\n");
}

void *childFunction(void *) {
	pthread_cleanup_push(cancellationHandler, 0);
	while(true) {
		printf("Hello\n");
	}
	pthread_cleanup_pop(0);
}

int main() {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, childFunction, NULL);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	pthread_cancel(thread_id);
}