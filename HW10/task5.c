#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PRINTER_COUNT 3
#define JOB_COUNT 14

sem_t printer_sem;
pthread_mutex_t active_mutex;

int current_active = 0;
int peak_active = 0;

void* print_task(void *arg) {
    long thread_id = (long)arg;

    sem_wait(&printer_sem);

    pthread_mutex_lock(&active_mutex);
    current_active++;
    if (current_active > peak_active) {
        peak_active = current_active;
    }
    printf("Thread %ld is printing... (active=%d)\n", thread_id, current_active);
    pthread_mutex_unlock(&active_mutex);

    usleep(200000);

    pthread_mutex_lock(&active_mutex);
    current_active--;
    pthread_mutex_unlock(&active_mutex);

    sem_post(&printer_sem);
    return NULL;
}

int main(void) {
    pthread_t jobs[JOB_COUNT];

    sem_init(&printer_sem, 0, PRINTER_COUNT);
    pthread_mutex_init(&active_mutex, NULL);

    for (long i = 0; i < JOB_COUNT; i++) {
        pthread_create(&jobs[i], NULL, print_task, (void*)i);
    }

    for (int i = 0; i < JOB_COUNT; i++) {
        pthread_join(jobs[i], NULL);
    }

    printf("Max simultaneously printing (should be ≤ %d): %d\n",
           PRINTER_COUNT, peak_active);

    sem_destroy(&printer_sem);
    pthread_mutex_destroy(&active_mutex);

    return 0;
}
