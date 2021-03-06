#include <stdio.h>
#include <stdlib.h>
#include "jval.h"

Jval new_jval_i(int i) { Jval ret; ret.i = i; return ret; }
Jval new_jval_l(long l) { Jval ret; ret.l= l; return ret; }
Jval new_jval_f(float f) { Jval ret; ret.f = f; return ret; }
Jval new_jval_d(double d) { Jval ret; ret.d = d; return ret; }
Jval new_jval_s(char *s) { Jval ret; ret.s = s; return ret; }
Jval new_jval_v(void *v) { Jval j; j.v = v; return j;}

int jval_i(Jval J) { return J.i; }
long jval_l(Jval J) { return J.l; }
float jval_f(Jval J) { return J.f; }
double jval_d(Jval J) { return J.d; }
void *jval_v(Jval J) { return J.v; }

/* buffer exch implemetion */
void exch(void *buf, int size, int i, int j)
{
	char *a = (char*)buf + i*size;
	char *b = (char*)buf + j*size;
	for (int k = 0; k < size; k++) {
		char tmp = a[k];
		a[k] = b[k];
		b[k] = tmp;
	}	
}

int search_gen ( Jval a[], int l, int r, Jval item, int (*compare_J)(Jval*, Jval*));
void sort_gen(Jval a[], int l, int r, int (*compare_J)(Jval*, Jval*))
{
	int i = l-1, j = r;
	int p = l-1, q = r;
	int size = sizeof(Jval);
	if (r <= l) return;
	while (1) {
		while (compare_J(a +(++i), a +(r)) < 0); // a[i] < a[r]
		while (compare_J(a +(r), a +(--j)) < 0) //a[r] < a[--j] 
			if (j == l)
				break;

		if (i >= j) break; // end of while(1) here
 
		exch(a, size, i, j);

		//swap left
		if (compare_J(a +(i), a +(r)) == 0) { // a[i] == a[r]
			p++;
			exch(a, size, p, i);
		}

		//swap right
		if (compare_J(a +(j), a +(r))== 0){ // a[j] == a[r]
			q--;
			exch(a, size, j, q);
		}
	}
 
	exch(a, size, i, r); 

	j = i-1;
	for (int k = l; k < p; k++, j--)
		exch(a, size, k, j);
 
	i = i+1;
	for (int k = r-1; k > q; k--, i++)
		exch(a, size, i, k);

	sort_gen(a, l, j, compare_J);
	sort_gen(a, i, r, compare_J);	
}


// integer
int compare_i(Jval* a, Jval* b) {
	if ( jval_i(*a)==jval_i(*b) ) return 0;
	if ( jval_i(*a) < jval_i(*b) ) return -1;
	else return 1;
}
void sort_i(Jval a[], int l, int r) {
	sort_gen(a, l, r, compare_i);
}
int search_i (Jval a[], int l, int r, int x) {
	return search_gen(a, l,r, new_jval_i(x), compare_i);
}

int search_gen(Jval a[], int l, int r, Jval item, int (*compare)(Jval*, Jval*))
{
	int i, res;
	if(r < l ) return -1;
	i = (l + r)/2;
	res = compare(&item, a + i);
	if(res == 0)
		return i;
	else if (res < 0)
		return search_gen(a, l, i - 1, item , compare);
	else
		return search_gen(a, i + 1, r, item, compare);
}

