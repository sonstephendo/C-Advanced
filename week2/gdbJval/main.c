#include <stdio.h>
#include <stdlib.h>
typedef union {
	int i;
	long l;
	float f;
	double d;
	void *v;
	char *s;
	char c;
}Jval ;


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

int compare_i(Jval* a, Jval* b) {
	if ( jval_i(*a)==jval_i(*b) ) return 0;
	if ( jval_i(*a) < jval_i(*b) ) return -1;
	else return 1;
}

int compare_f(Jval* a, Jval* b) {
	if ( jval_f(*a)==jval_f(*b) ) return 0;
	if ( jval_f(*a) < jval_f(*b) ) return -1;
	else return 1;
}

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
Jval* create_array_i (int n) {
	Jval * array = (Jval *) malloc(sizeof(Jval)*n);
	for (int i=0; i<n; i++) array[i] = new_jval_i( 1+rand()%100 );
	return array;
}
Jval* create_array_f (int n) {
	Jval * array = (Jval *) malloc(sizeof(Jval)*n);
	for (int i=0; i<n; i++) array[i] = new_jval_f(((float)rand()/(float)(RAND_MAX)) * 100.0);
	return array;
}

void sort_gen(Jval a[], int l, int r, int (*compare_J)(Jval*, Jval*))
{
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	int size = sizeof(Jval);
	while (1) {
		while (compare_J(a +(++i), a +(r)) < 0); // a[i] < a[r]
		while (compare_J(a +(r), a +(--j)) < 0) //a[r] < a[--j]
			if (j == l) break;
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
			exch(a, size, q, j);
		}
	}
	exch(a, size, i, r);
	j = i-1;
	for (int k = l; k <= p; k++, j--)
		exch(a, size, k, j);

	i = i+1;
	for (int k = r-1; k >= q; k--, i++)
		exch(a, size, i, k);

	sort_gen(a, l, j, compare_J);
	sort_gen(a, i, r, compare_J);
}
void printarr_i(Jval a[], int size) {
	printf("Show Array: ");
	for (int i = 0; i < size; i++)
		printf("%d  ", jval_i(*(a+i)));
	printf("\n");
}
void printarr_f(Jval a[], int size) {
	printf("Show Array: ");
	for (int i = 0; i < size; i++)
		printf("%.2f  ", jval_f(*(a+i)));
	printf("\n");
}

void sort_i(Jval a[], int l, int r) {
	sort_gen(a, l, r, compare_i);
}
void sort_f(Jval a[], int l, int r) {
	sort_gen(a, l, r, compare_f);
}

int main()
{
	Jval *a = create_array_i(10);
	/* Jval *b = create_array_f(10); */
	printarr_i(a, 10);
	/* printarr_f(b, 10); */
	sort_i(a, 0, 10-1);
	/* sort_f(b,0, 10-1); */
	printarr_i(a, 10);
	/* printarr_f(b, 10); */
	/* int com = compare_i((char*)a, (char*)a +1*(sizeof(Jval))); */
	//printf("%f \n", ((float)rand()/(float)(RAND_MAX)) * 100.0);
	free(a);
	/* free(b); */
	return 0;
}
