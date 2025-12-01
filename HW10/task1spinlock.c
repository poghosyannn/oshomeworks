#include <stdio.h>
#include <pthread.h>

long long counter = 0;
int M = 100000;
int N = 6;

pthread_spinlock_t counter_spin;

void* increment(void *arg) {
    int j;
    for (j = 0; j < M; j++) {
        pthread_spin_lock(&counter_spin);
        counter++;
        pthread_spin_unlock(&counter_spin);
    }
    return NULL;
}

int main() {
    pthread_t t[N];
    int i;

    if (pthread_spin_init(&counter_spin, PTHREAD_PROCESS_PRIVATE) != 0) {
        perror("spin init failed");
        return 1;
    }

    printf("Starting spinlock-protected counter test...\n");

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

    pthread_spin_destroy(&counter_spin);
    return 0;
}
