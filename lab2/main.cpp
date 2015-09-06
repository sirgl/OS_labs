#include <pthread.h>
#include <iostream>

using namespace std;

void *childFunction(void *) {
	for (int i = 0; i < 10; ++i) {
		cout << "Hello, it's child!" << endl;
	}
}

int main() {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, childFunction, NULL);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 10; ++i) {
		cout << "Hello, it's parent!" << endl;
	}
	pthread_join(thread_id, NULL);
}