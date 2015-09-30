#include <pthread.h>
#include <iostream>

using namespace std;

void *calc(void *) {
	int i;
	for (i = 0; i < 10; ++i) {
		cout << "Hello, it's child!" << endl;
	}
	return (void *) i;
}

int main() {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, calc, NULL);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 10; ++i) {
		cout << "Hello, it's parent!" << endl;
	}
	void** ret;
	pthread_join(thread_id, ret);
	printf("%d", *ret);
}