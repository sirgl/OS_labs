#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int ITERATIONS = 10000000;


int threadCount;

struct Parameter {
	int index;
	double result;
};

void *calc(void *param) {
	Parameter *parameter = (Parameter *) param;
	double result = 0;
//	for (int i = parameter->index; i < ITERATIONS; i += threadCount) {
//		result += 1.0 / (i * 4.0 + 1.0);
//		result -= 1.0 / (i * 4.0 + 3.0);
//	}
	int start = ITERATIONS / threadCount * parameter->index ;
	int end = parameter->index == threadCount - 1 ? ITERATIONS : ITERATIONS / threadCount * (parameter->index + 1);
	for (int i = start; i < end; ++i) {
		result += 1.0 / (i * 4.0 + 1.0);
		result -= 1.0 / (i * 4.0 + 3.0);
	}
	parameter->result = result;
	return parameter;
}

int main(int argc, char **argv) {
	if (2 != argc) {
		fprintf(stderr, "Not enough arguments");
		exit(EXIT_FAILURE);
	}

	threadCount = atoi(argv[1]);

	pthread_t threads[threadCount];
	Parameter **parameters = (Parameter **) malloc(sizeof(Parameter *) * threadCount);
	if (NULL == parameters) {
		fprintf(stderr, "Not enough memory");
		exit(EXIT_FAILURE);
	}
	for (int j = 0; j < threadCount; ++j) {
		parameters[j] = (Parameter *) malloc(sizeof(Parameter));
		if (NULL == parameters[j]) {
			fprintf(stderr, "Not enough memory");
			exit(EXIT_FAILURE);
		}
		parameters[j]->index = j;
		parameters[j]->result = 0;
	}


	for (int i = 0; i < threadCount; ++i) {
		int error = pthread_create(&threads[i], NULL, calc, parameters[i]);
		if (error) {
			fprintf(stderr, "error while creating threads");
			exit(EXIT_FAILURE);
		}
	}

	double pi = 0;

	for (int i = 0; i < threadCount; ++i) {
		Parameter *retValue;
		int error = pthread_join(threads[i], (void **) &retValue);
		if (error) {
			fprintf(stderr, "error while creating threads");
			exit(EXIT_FAILURE);
		}
		pi += retValue->result;
		free(retValue);
	}
	pi *= 4.0;
	printf("%.10lf\n", pi);
	free(parameters);
	return EXIT_SUCCESS;
}