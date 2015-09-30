#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void *calc(void *type) {
	while(1) {
		write(1, "Hello\n", 7);
	}
}

int main() {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, calc, NULL);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	pthread_cancel(thread_id);
}