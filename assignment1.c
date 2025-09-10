#include <stdio.h>

int main() {

    int n = 2;
    int *p = &n;

    printf("Address: %d \n", &n);
    printf("Address: %d \n", p);
    *p = 3;

    printf("Changed Value: %d \n", n);
    
}
