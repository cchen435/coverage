int add(int a[], int b[], int c[], int d[], int size) {

	int i = 0, j = size, k=i+j, l, m, n;
//    m = n = k = i = 5;
	l = m+k+i+j;

	for (i = 0; i < size/2; i++) {
		c[i] = a[2*i] + b[2*i+1]+a[i*4];
		d[i] = a[2*i+1] + b[2*i];
	}


    if ( i > j ) {
        i = j+ 4;
        j = j + k;
    } else {
        l = m+4;
        n = k + i;
    }
    
    if ( l > m ) {
        i = j+ 4;
        j = j + k;
    }

    while ( i < j ) {
        i++;
        m+= n;
    }
	return 0;
}
