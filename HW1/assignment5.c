#include <stdio.h>

int main() {
    char str[] = "HelloDearInstructorOrTA";
    char *p = str;
    int count = 0;

    printf("Printing the string using a pointer and a loop:\n");
    while (*p != '\0') {
        printf("%c", *p);
        count++;
        p++;
    }

    printf("\n");
    printf("Count: %d\n", count);
}
