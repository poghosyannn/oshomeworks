#include <stdio.h>
#include <pthread.h>

long long counter = 0;
int M = 100000;
int N = 6;

pthread_mutex_t counter_lock;

void* increment(void *arg) {
    int j;
    for (j = 0; j < M; j++) {
        pthread_mutex_lock(&counter_lock);
        counter++;
        pthread_mutex_unlock(&counter_lock);
    }
    return NULL;
}

int main() {
    pthread_t t[N];
    int i;

    if (pthread_mutex_init(&counter_lock, NULL) != 0) {
        perror("mutex init failed");
        return 1;
    }

    printf("Starting mutex-protected counter test...\n");

    for (i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, increment, NULL) != 0) {
            perror("thread creation failed");
            return 1;
        }
    }

    for (i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }

    printf("Expected output: %d\n", M * N);
    printf("Actual output: %lld\n", counter);

    pthread_mutex_destroy(&counter_lock);
    return 0;
}
