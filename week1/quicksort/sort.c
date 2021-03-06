#include <stdio.h>
#include "sort.h"
#include <stdlib.h>
// Comparation for qsort in standard libary
int compare(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

/* 2-way quick sort implementation */
void swap(int *a, int *b) {
	int c = *a;
	*a = *b;
	*b = c;
}
 
void quicksort2 (int arr[], int low, int high)
{
	if (low >= high) return;
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
	i = i + 1;
	
	quicksort2(arr, low, i - 1);
	quicksort2(arr, i + 1, high);
}


//3-way tranfering...
void quicksort3(int a[], int l, int r)
{
    int i = l-1, j = r;
    int p = l-1, q = r;
    int v = a[r];
	if (r <= l) return;
    while (1) {
        // From left, find the first element greater than
        // or equal to v. This loop will definitely terminate
        // as v is last element
        while (a[++i] < v);
 
        // From right, find the first element smaller than or
        // equal to v
        while (v < a[--j])
            if (j == l)
                break;
 
        // If i and j cross, then we are done
        if (i >= j) break;
 
        // Swap, so that smaller goes on left greater goes on right
		swap(&a[i], &a[j]);
 
        // Move all same left occurrence of pivot to beginning of
        // array and keep count using p
        if (a[i] == v)
        {
            p++;
            swap(&a[p], &a[i]);
        }
 
        // Move all same right occurrence of pivot to end of array
        // and keep count using q
        if (a[j] == v)
        {
            q--;
            swap(&a[j], &a[q]);
        }
    }
 
    // Move pivot element to its correct index
    swap(&a[i], &a[r]);
 
    // Move all left same occurrences from beginning
    // to adjacent to arr[i]
    j = i-1;
    for (int k = l; k < p; k++, j--)
         swap(&a[k], &a[j]);
 
    // Move all right same occurrences from end
    // to adjacent to arr[i]
    i = i+1;
    for (int k = r-1; k > q; k--, i++)
		swap(&a[i], &a[k]);

	//recursion
	quicksort3(a, l, j);
    quicksort3(a, i, r);
	
}
