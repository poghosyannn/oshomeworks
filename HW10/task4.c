#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define ITERATIONS 10

sem_t semA, semB, semC;

void* run_A(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(&semA);
        printf("A %d\n", i);
        sem_post(&semB);
    }
    return NULL;
}

void* run_B(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(&semB);
        printf("B %d\n", i);
        sem_post(&semC);
    }
    return NULL;
}

void* run_C(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(&semC);
        printf("C %d\n", i);
        sem_post(&semA);
    }
    return NULL;
}

int main(void) {
    pthread_t thA, thB, thC;

    sem_init(&semA, 0, 1);
    sem_init(&semB, 0, 0);
    sem_init(&semC, 0, 0);

    if (pthread_create(&thA, NULL, run_A, NULL) != 0) {
        perror("pthread_create A");
        return 1;
    }
    if (pthread_create(&thB, NULL, run_B, NULL) != 0) {
        perror("pthread_create B");
        return 1;
    }
    if (pthread_create(&thC, NULL, run_C, NULL) != 0) {
        perror("pthread_create C");
        return 1;
    }

    pthread_join(thA, NULL);
    pthread_join(thB, NULL);
    pthread_join(thC, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}
