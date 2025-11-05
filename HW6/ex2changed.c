#include <stdio.h>

struct my_struct {
    char   a;
    double c;
    int    b;
};

int main(void) {
    struct my_struct A = {0};
    printf("Size of my_struct: %zu\n", sizeof(struct my_struct));
    printf("Address of char:   %p\n", (void*)&A.a);
    printf("Address of int:    %p\n", (void*)&A.b);
    printf("Address of double: %p\n", (void*)&A.c);
    return 0;
}

