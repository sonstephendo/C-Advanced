#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SMALL_NUMBER 20
#define HUGE_NUMBER 1000000

#include "sort.h"

int* createArray(int size);
int* dumpArray(int *p, int size);

void printarr(int a[], int size);
void measuretime(void(*f)(int*, int, int), int a[], int size);

int main()
{
	int *a1, *a2, *a3;
	a1 = createArray(SMALL_NUMBER);
	a2 = dumpArray(a1, SMALL_NUMBER);
	a3 = dumpArray(a1, SMALL_NUMBER);
	printf("Initial\n");
	printarr(a1, SMALL_NUMBER);
	printarr(a2, SMALL_NUMBER);
	printarr(a3, SMALL_NUMBER);
	
	printf("After sorting\n");
	quicksort2(a1, 0, SMALL_NUMBER - 1);
	/* print data in a1 */
	printarr(a1, SMALL_NUMBER);
	
	quicksort3(a2, 0, SMALL_NUMBER - 1);
	/* print data in a2 */
	printarr(a2, SMALL_NUMBER);

	qsort(a3, SMALL_NUMBER, sizeof(int), compare);
	/* print data in a3 */
	printarr(a3, SMALL_NUMBER);
	free(a1);
	free(a2);
	free(a3);
	
	a1 = createArray(HUGE_NUMBER);
	a2 = dumpArray(a1, HUGE_NUMBER);
	a3 = dumpArray(a1, HUGE_NUMBER);
	
	printf("Quick Sort 3-way partions\n");
	measuretime(quicksort3, a2, HUGE_NUMBER);
	printf("Quick Sort 2-way partions\n");
	measuretime(quicksort2, a1, HUGE_NUMBER);
	printf("Quick Sort qsort built-in\n");
	clock_t start, end;

	start = clock();
  	//function test	
	qsort(a3, HUGE_NUMBER, sizeof(int), compare);
	end = clock();
	double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\tRun time: %.2lf seconds \n",time_taken);
	free(a1);
	free(a2);
	free(a3);
	
    return 0;
}

// A utility function to print an array
void printarr(int a[], int size) {
	printf("Show Array: ");
	for (int i = 0; i < size; i++)
		printf("%d  ", a[i]);
	printf("\n");
}

void measuretime(void(*f)(int*, int, int), int a[], int size)
{
	clock_t start, end;

	start = clock();
  	//function test	
	(*f)(a, 0, size-1);
	end = clock();
	double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Run time: %.2f seconds \n",time_taken);
}

int* createArray(int size) {

	// seed for random input
	srand(time(NULL));
	int* a = (int*)malloc(size * sizeof(int));
	for(int i = 0; i < size; i++) {
		a[i] = 1 + rand() % 100;
	}
	return a;
}

int* dumpArray(int *p, int size) {
	int* a = (int*)malloc(size * sizeof(int));
	memcpy(a, p, size* sizeof(int));
	return a;
}
