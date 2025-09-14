#include "stdio.h"

int main(){
    int n = 18;
    int *p = &n;
    int **doublep = &p;

    printf("The Value from Pointer: %d\n", *p);
    printf("The Value from Double Dointer: %d \n", **doublep);
}