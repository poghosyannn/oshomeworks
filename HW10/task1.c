#include <stdio.h>
#include <pthread.h>

long long counter = 0;
int M = 100000;
int N = 6;

void* increment(void *arg) {
    int j;
    for (j = 0; j < M; j++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t t[N];
    int i;

    printf("Starting unsynchronized counter test...\n");

    for (i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, increment, NULL) != 0) {
            perror("thread creation failed");
            return 1;
        }
    }

    for (int i = 0; i < N; i++) {
        if (pthread_join(t[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }

    printf("Expected output: %d\n", M * N);
    printf("Actual output: %lld\n", counter);

    return 0;
}
