#include <stdio.h>
#include <stdlib.h>

void cleanup1() {
    printf("cleanup1 executed\n");
}

void cleanup2() {
    printf("cleanup2 executed\n");
}

int main() {
    atexit(cleanup1);
    atexit(cleanup2);
    exit(0);
}
