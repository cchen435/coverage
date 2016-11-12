#include <stdio.h>


struct rect {
	int a;
	int b;
};

int area(struct rect r) {
	int w = r.a;
	int l = r.b;
	int size = w * l;
	return size;
}

int main(int argc, char **argv) {
	int w = 20, l = 30; 
	int size = 0;
	struct rect r;
	r.a = w;
	r.b = l;
	size = area(r);
	return 0;
}
