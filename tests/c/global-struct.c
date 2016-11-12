#include <stdio.h>


struct rect {
	int a;
	int b;
	int area_size;
} r;

int area(struct rect shape) {
	int w = shape.a;
	int l = shape.b;
	int size = w * l;
	return size;
}

int main(int argc, char **argv) {
	int w = 20, l = 30; 
	int size = 0;
	r.a = w;
	r.b = l;
	size = area(r);
	r.area_size = size;
	return 0;
}
