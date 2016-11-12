#include <stdio.h>

void swap( int *a, int i, int j) {
	int tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

void add( int *a, int *b, int *c, int size) {
	int i;
	for (i = 0; i < size; i++) {
		c[i] = a[i]+b[i];
	}
}

int sort(int *a, int size) {
	int i, j;
	for ( i = 0; i < size -1; i++) {
		for (j = i+1; j < size; j++) {
			if (a[i] > a[j]) {
				swap(a, i, j);
			} 
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	int array_a[10] = {5, 4, 3, 2, 0, 8, 6, 9, 1, 7};
	int array_b[10] = {5, 4, 3, 2, 0, 8, 6, 9, 1, 7};
	int array_c[10];
	sort(array_a, 10);
	add(array_a, array_b, array_c, 10);
	return 0;
}
