#include <stdio.h>
#include <pthread.h>

void* run_numbers(void *arg) {
    int id = *(int*)arg;
    printf("Thread %d running:\n", id);

    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");
    return NULL;
}

int main() {
    pthread_t th[3];
    int ids[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&th[i], NULL, run_numbers, &ids[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    return 0;
}
