#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TOTAL_ITEMS     100
#define BUFFER_SIZE     8
#define PRODUCER_COUNT  4
#define CONSUMER_COUNT  2

int buffer[BUFFER_SIZE];
int write_pos = 0;
int read_pos = 0;
int remaining_items = TOTAL_ITEMS;

sem_t sem_empty;
sem_t sem_full;
pthread_mutex_t buf_mutex;

void print_buffer_state(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", buffer[i]);
    }
    printf("\n");
}

void* producer_thread(void *arg) {
    while (1) {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&buf_mutex);

        if (remaining_items <= 0) {
            pthread_mutex_unlock(&buf_mutex);
            sem_post(&sem_empty);
            break;
        }

        buffer[write_pos] = 1;
        write_pos = (write_pos + 1) % BUFFER_SIZE;
        remaining_items--;

        pthread_mutex_unlock(&buf_mutex);
        sem_post(&sem_full);
    }
    return NULL;
}

void* consumer_thread(void *arg) {
    while (1) {
        sem_wait(&sem_full);
        pthread_mutex_lock(&buf_mutex);

        int value = buffer[read_pos];
        if (value == -1) {
            pthread_mutex_unlock(&buf_mutex);
            sem_post(&sem_full);
            break;
        }

        buffer[read_pos] = 0;
        print_buffer_state();
        read_pos = (read_pos + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buf_mutex);
        sem_post(&sem_empty);
    }
    return NULL;
}

int main(void) {
    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    sem_init(&sem_empty, 0, BUFFER_SIZE);
    sem_init(&sem_full, 0, 0);
    pthread_mutex_init(&buf_mutex, NULL);

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        if (pthread_create(&producers[i], NULL, producer_thread, NULL) != 0) {
            perror("pthread_create producer");
            exit(1);
        }
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        if (pthread_create(&consumers[i], NULL, consumer_thread, NULL) != 0) {
            perror("pthread_create consumer");
            exit(1);
        }
    }

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&buf_mutex);

        buffer[write_pos] = -1;
        write_pos = (write_pos + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buf_mutex);
        sem_post(&sem_full);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&buf_mutex);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);

    return 0;
}
