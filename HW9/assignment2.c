#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int left;
    int right;
    int *data;
    int sum;
} Task;

void* sum_worker(void *arg) {
    Task *t = (Task*)arg;
    int s = 0;
    for (int i = t->left; i <= t->right; i++) {
        s += t->data[i];
    }
    t->sum = s;
    return NULL;
}

int main() {
    int n;
    printf("Enter array length:\n");
    scanf("%d", &n);

    int *arr = malloc(n * sizeof(int));
    if (!arr) return 1;

    printf("Enter array values:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    Task t1, t2;
    t1.left = 0;
    t1.right = (n - 1) / 2;
    t1.data = arr;
    t1.sum = 0;

    t2.left = t1.right + 1;
    t2.right = n - 1;
    t2.data = arr;
    t2.sum = 0;

    pthread_t th1, th2;

    if (pthread_create(&th1, NULL, sum_worker, &t1) != 0) return 1;
    if (pthread_create(&th2, NULL, sum_worker, &t2) != 0) return 1;

    if (pthread_join(th1, NULL) != 0) return 1;
    if (pthread_join(th2, NULL) != 0) return 1;

    printf("partial sum 1: %d\n", t1.sum);
    printf("partial sum 2: %d\n", t2.sum);

    free(arr);
    return 0;
}
