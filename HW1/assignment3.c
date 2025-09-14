#include <stdio.h>

void swap(int *a, int *b) {
    int t = *a; 
    *a = *b;
    *b = t;
}

int main() {
    int a = 1, b = 11;

    printf("Before swapping - ");
    printf("a: %d, b: %d\n", a, b);

    swap(&a, &b);

    printf("After swapping - ");
    printf("a: %d, b: %d\n", a, b);
}
