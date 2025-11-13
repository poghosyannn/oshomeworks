#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* square_worker(void *arg) {
    int value = *(int *)arg;
    printf("Square of %d is %d\n", value, value * value);
    return NULL;
}

int main() {
    int n;
    printf("Enter array size: ");
    scanf("%d", &n);

    int *nums = malloc(n * sizeof(int));
    if (nums == NULL) {
        perror("malloc");
        return 1;
    }

    printf("Enter the numbers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &nums[i]);
    }

    pthread_t *th = malloc(n * sizeof(pthread_t));
    if (th == NULL) {
        perror("malloc");
        free(nums);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (pthread_create(&th[i], NULL, square_worker, &nums[i]) != 0) {
            perror("pthread_create");
            free(nums);
            free(th);
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        pthread_join(th[i], NULL);
    }

    free(nums);
    free(th);
    return 0;
}
