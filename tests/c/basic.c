int add(int a[], int b[], int c[], int d[], int size) {
	int i = 0, j = size, k=i+j, l, m;
	l = m+k;
	for (i = 0; i < size/2; i++) {
		c[i] = a[2*i] + b[2*i+1];
		d[i] = a[2*i+1] + b[2*i];
	}
	return 0;
}
