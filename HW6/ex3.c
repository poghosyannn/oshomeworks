#include <stdio.h>

struct my_struct {
    char   a;
    int    b;
    double c;
};

int main(void) {
    struct my_struct A = {0};
    printf("Size of my_struct: %lu\n", sizeof(A));
    printf("Address of char:   %p\n", (void*)&A.a);
    printf("Address of int:    %p\n", (void*)&A.b);
    printf("Address of double: %p\n", (void*)&A.c);
    return 0;
}

