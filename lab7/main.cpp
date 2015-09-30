#include <pthread.h>
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>

const int ITERATIONS = 1000000;

struct Parameter{
	int index;
	double result;
};

void cancellationHandler(void*) {
	
}

void *calc(void *param) {
	Parameter* parameter = (Parameter *) param;
	double result = 0;
	for (int i = 0; i < ITERATIONS; ++i) {
		result += 1.0 / (i * 4.0 + 1.0);
		result -= 1.0 / (i * 4.0 + 3.0);
	}
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