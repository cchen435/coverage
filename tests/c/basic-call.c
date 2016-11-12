#include <stdio.h>


int mul(int a, int b) {
	return a * b;
}

int func(int a) {
    int b = a + 3;
    a = a+4;
    int c = a * 8;
    int d, e;

    if ( b > 10 ) {
        d = a + b;
    } else {
        d = a * b;
    }
    //e = d * a;
	e = mul(d, a);
    return e;
}

int main(int argc, char **argv) {
    int a = func(10);
	printf("Hello World: %d\n", a);
	return 0;
}
