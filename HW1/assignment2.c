#include "stdio.h"

int main(){
    int arr[] = {0, 5, 10, 15, 20};
    int *p_arr = arr;

    printf("Initial array: ");
    for(int i=0; i<5; i++){
        printf("%d ", *(p_arr+i));
    }

    for (int i = 0; i<5; i++) {
        *(p_arr+i) = *(p_arr+i)+1;
    }
    printf("\n");
    printf("Modified array: ");

    for(int i=0; i<5; i++){
        printf("%d ", *(arr+i));
    }
    printf("\n");
    printf("Array-based print: ");
    for (int i = 0; i<5; i++) {
        printf("%d ", arr[i]);
    }
}
