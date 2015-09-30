#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void cancellationHandler(void *) {
	printf("interrupted!\n");
}

void *calc(void *) {

}

int main() {
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	int trylock = pthread_mutex_trylock(&mutex);

	if (trylock == EDEADLK) {
	}
	printf("%d", trylock);
	pthread_mutex_unlock(&mutex);
}