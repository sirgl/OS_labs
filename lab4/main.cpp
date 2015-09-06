#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void *childFunction(void *) {
	while(true) {
		printf("Hello\n");
	}
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