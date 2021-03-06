#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SMALL_NUMBER 20
#define HUGE_NUMBER 1000000
#include "jval.h"

Jval *create_array_i (int n) {
	srand(time(NULL));
	Jval * array = (Jval *) malloc(sizeof(Jval)*n);
	for (int i=0; i<n; i++)
	array[i] = new_jval_i(1 + rand()%100 );
	return array;
}

int* dumpArray(int *p, int size);

void printarr(Jval a[], int size);

int main()
{
	Jval *a1;
	a1 = create_array_i(SMALL_NUMBER);
	
	printf("Initial\n");
	printarr(a1, SMALL_NUMBER);
	
	printf("After sorting\n");
	sort_i(a1, 0, SMALL_NUMBER);
	/* print data in a1 */
	printarr(a1, SMALL_NUMBER);
	

	/* print data in a2 */
	//printarr(a2, SMALL_NUMBER);
	
	free(a1);
	//free(a2);
	
	a1 = create_array_i(HUGE_NUMBER);
	//a2 = dumpArray(a1, HUGE_NUMBER);
	
	clock_t start, end;
	double time_taken;
	printf("Quick Sort time measure\n");
	start = clock();
  	//function test	
	sort_i(a1, 0, HUGE_NUMBER);
	end = clock();
	time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\tRun time: %.2lf seconds \n",time_taken);
	int rand_num = 1 + rand()%100;
	int s_location = search_i(a1, 0, HUGE_NUMBER - 1, rand_num);
	if(s_location!= -1)
		printf("%d locates in array: %d \n", rand_num, s_location);
	else
		printf("Can't find the item!\n");
	printf("Search checking:arr[%d] =  %d\n", s_location,  jval_i(a1[s_location]));

	free(a1);
	//free(a2);

	
    return 0;
}

// A utility function to print an array
void printarr(Jval a[], int size) {
	printf("Show Array: ");
	for (int i = 0; i < size; i++)
		printf("%d  ", jval_i(*(a+i)));
	printf("\n");
}

int* dumpArray(int *p, int size) {
	int* a = (int*)malloc(size * sizeof(int));
	memcpy(a, p, size* sizeof(int));
	return a;
}
