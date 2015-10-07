#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

const char* SEMAPHORE_NAME_1 = "/s1";
const char* SEMAPHORE_NAME_2 = "/s2";

int main(int argc, char **argv) {
	sem_t* semaphore[2];

	if(NULL == (semaphore[0] = sem_open(SEMAPHORE_NAME_1, O_CREAT, 0600, 0))) {
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	if(NULL == (semaphore[1] = sem_open(SEMAPHORE_NAME_2, O_CREAT, 0600, 1))) {
		perror("sem_open");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	if(-1 == (pid = fork())) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid == 0) {
		for(int i = 0; i < 10; i++) {
			sem_wait(semaphore[0]);
			printf("Child %d\n", i);
			sem_post(semaphore[1]);
		}
	} else {
		for(int i = 0; i < 10; i++) {
			sem_wait(semaphore[1]);
			printf("Parent %d\n", i);
			sem_post(semaphore[0]);
		}

	}

	sem_unlink(SEMAPHORE_NAME_1);
	sem_unlink(SEMAPHORE_NAME_2);
	return EXIT_SUCCESS;
}