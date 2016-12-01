int add(int a[], int b[], int c[], int size) {

    int i;
	
    for (i = 0; i<size/2; i++) {
        c[i] = a[i] + b[2*i+1+4*i];
    }

	return 0;
}
