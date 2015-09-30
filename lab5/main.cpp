#include <pthread.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>

void cancellationHandler(void*) {
	write(1, "interupted\n", 12);
}

void *calc(void *ds) {
	pthread_cleanup_push(cancellationHandler, 0);
	while(1) {
		write(1, "Hello\n", 7);
	}
	pthread_cleanup_pop(0);
}

int main() {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, calc, NULL);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	pthread_cancel(thread_id);
	pthread_join(thread_id, NULL);
}