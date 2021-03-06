#ifndef _JVAL_H
#define _JVAL_H
typedef union {
	int i;
	long l;
	float f;
	double d;
	void *v;
	char *s;
	char c;
}Jval ;

Jval new_jval_i(int i);
Jval new_jval_f(float f);
Jval new_jval_d(double d);
Jval new_jval_s(char *s);

int jval_i(Jval J);
float jval_f(Jval J);
double jval_d(Jval J);
char *jval_s(Jval J);
void exch(void *buf, int size, int i, int j);
void sort_gen(Jval a[], int l, int r, int (*compare_J)(Jval*, Jval*));
int search_gen ( Jval a[], int l, int r, Jval item, int (*compare_J)(Jval*, Jval*));
// integer
int compare_i(Jval* a, Jval* b);
void sort_i (Jval a[], int l, int r);
int search_i (Jval a[], int l, int r, int x);
#endif
/* _JVAL_H */


