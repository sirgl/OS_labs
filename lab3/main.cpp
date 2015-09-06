#include <pthread.h>
#include <string.h>
#include <cstdio>
#include <stdlib.h>

typedef struct StringArray {
	char** strings;
	size_t capacity;
	size_t size;
} StringArray;

void *childFunction(void *stringArray) {
	StringArray* arr = (StringArray *) stringArray;
	char **strings = arr->strings;
	for (int i = 0; i < arr->size; ++i) {
		printf("%s\n", strings[i]);
	}
	return stringArray;
}

pthread_t runPrintingThread(StringArray *arr) {
	pthread_t thread_id;
	int code = pthread_create(&thread_id, NULL, childFunction, arr);
	if (code != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	return thread_id;
}

StringArray * getStringsArray(size_t size) {
	StringArray *arr = (StringArray *) malloc(sizeof(StringArray));
	if(NULL == arr) {
		return NULL;
	}
	char **strings = (char **) malloc(size * sizeof(char*));
	if(NULL == strings) {
		free(arr);
		return NULL;
	}
	arr->capacity = size;
	arr->size = 0;
	arr->strings = strings;
	return arr;
}

int addStringToArray(StringArray* array, char* string) {
	if(array->size == array->capacity) {
		return 1;
	}
	char *newStr = (char *) malloc((strlen(string) + 1) * sizeof(char));
	if(NULL == newStr) {
		return 2;
	}
	strcpy(newStr, string);
	array->strings[array->size] = newStr;
	array->size++;
	return 0;
}

void freeStringArray(StringArray* stringArray){
	char **strings = stringArray->strings;
	for (int i = 0; i < stringArray->size; ++i) {
		free(strings[i]);
	}
	free(strings);
}

int main() {
	char str1[] = "hello";
	char str2[] = "beautiful";
	char str3[] = "graceful";
	char str4[] = "world";

	StringArray *array1 = getStringsArray(2);
	StringArray *array2 = getStringsArray(4);
	StringArray *array3 = getStringsArray(3);
	StringArray *array4 = getStringsArray(3);

	addStringToArray(array1, str1);
	addStringToArray(array1, str4);
	
	addStringToArray(array2, str1);
	addStringToArray(array2, str2);
	addStringToArray(array2, str3);
	addStringToArray(array2, str4);

	addStringToArray(array3, str1);
	addStringToArray(array3, str2);
	addStringToArray(array3, str4);

	addStringToArray(array4, str1);
	addStringToArray(array4, str3);
	addStringToArray(array4, str4);

	auto t1 = runPrintingThread(array1);
	auto t2 = runPrintingThread(array2);
	auto t3 = runPrintingThread(array3);
	auto t4 = runPrintingThread(array4);
	StringArray* array;
	pthread_join(t1, (void **) &array);
	freeStringArray(array);
	pthread_join(t2, (void **) &array);
	freeStringArray(array);
	pthread_join(t3, (void **) &array);
	freeStringArray(array);
	pthread_join(t4, (void **) &array);
	freeStringArray(array);
	pthread_exit(NULL);
}