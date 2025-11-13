#include <stdio.h>
#include <pthread.h>

void* run_thread(void *arg) {
    pthread_t id = pthread_self();
    int num = *(int*)arg;
    printf("Thread %d is now running (id=%ld)\n", num, id);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    int a = 1, b = 2, c = 3;

    if (pthread_create(&t1, NULL, run_thread, &a) != 0) return 1;
    if (pthread_create(&t2, NULL, run_thread, &b) != 0) return 1;
    if (pthread_create(&t3, NULL, run_thread, &c) != 0) return 1;

    if (pthread_join(t1, NULL) != 0) return 1;
    if (pthread_join(t2, NULL) != 0) return 1;
    if (pthread_join(t3, NULL) != 0) return 1;

    return 0;
}
