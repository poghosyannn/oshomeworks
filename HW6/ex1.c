#include <stdio.h>

int main(void) {
    int    i = 32;
    char   c = 'j';
    double d = 2.43;
    short  s = 3;

    printf("Sizeof int: %lu, address: %p\n",    sizeof(i), (void*)&i);
    printf("Sizeof char: %lu, address: %p\n",   sizeof(c), (void*)&c);
    printf("Sizeof double: %lu, address: %p\n", sizeof(d), (void*)&d);
    printf("Sizeof short: %lu, address: %p\n",  sizeof(s), (void*)&s);
    return 0;
}

