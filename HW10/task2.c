#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

long long balance = 0;
int num_increments = 0;
int num_decrements = 0;
int num_steps = 0;
int mode = 0;                /* 0 = short, 1 = long */

pthread_mutex_t mutex;
pthread_spinlock_t spinlock;

void* inc_mutex(void *arg) {
    for (int i = 0; i < num_steps; i++) {
        pthread_mutex_lock(&mutex);
        balance++;
        if (mode) {
            usleep(100);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* dec_mutex(void *arg) {
    for (int i = 0; i < num_steps; i++) {
        pthread_mutex_lock(&mutex);
        balance--;
        if (mode) {
            usleep(100);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* inc_spin(void *arg) {
    for (int i = 0; i < num_steps; i++) {
        pthread_spin_lock(&spinlock);
        balance++;
        if (mode) {
            usleep(100);
        }
        pthread_spin_unlock(&spinlock);
    }
    return NULL;
}

void* dec_spin(void *arg) {
    for (int i = 0; i < num_steps; i++) {
        pthread_spin_lock(&spinlock);
        balance--;
        if (mode) {
            usleep(100);
        }
        pthread_spin_unlock(&spinlock);
    }
    return NULL;
}

double run_test(void *(*inc_func)(void *), void *(*dec_func)(void *)) {
    int total_threads = num_increments + num_decrements;
    pthread_t threads[total_threads];
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (int i = 0; i < num_increments; i++) {
        if (pthread_create(&threads[i], NULL, inc_func, NULL) != 0) {
            perror("pthread_create (inc)");
            exit(1);
        }
    }

    for (int i = num_increments; i < total_threads; i++) {
        if (pthread_create(&threads[i], NULL, dec_func, NULL) != 0) {
            perror("pthread_create (dec)");
            exit(1);
        }
    }

    for (int i = 0; i < total_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    gettimeofday(&end, NULL);

    double ms = (end.tv_sec - start.tv_sec) * 1000.0;
    ms += (end.tv_usec - start.tv_usec) / 1000.0;
    return ms;
}

int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr,
                "Usage: %s <mutex|spinlock> <short|long> <num_inc_threads> <num_dec_threads> <steps_per_thread>\n",
                argv[0]);
        return 1;
    }

    char *sync_mechanism = argv[1];
    char *mode_str = argv[2];

    if (strcmp(mode_str, "short") == 0) {
        mode = 0;
    } else if (strcmp(mode_str, "long") == 0) {
        mode = 1;
    } else {
        fprintf(stderr, "mode must be 'short' or 'long'\n");
        return 1;
    }

    num_increments = atoi(argv[3]);
    num_decrements = atoi(argv[4]);
    num_steps      = atoi(argv[5]);

    if (num_increments < 0 || num_decrements < 0 || num_steps <= 0) {
        fprintf(stderr, "invalid numeric arguments\n");
        return 1;
    }

    printf("mode=%s, inc_threads=%d, dec_threads=%d, steps=%d\n",
           mode_str, num_increments, num_decrements, num_steps);

    double elapsed_ms = 0.0;

    if (strcmp(sync_mechanism, "mutex") == 0) {
        if (pthread_mutex_init(&mutex, NULL) != 0) {
            perror("pthread_mutex_init");
            return 1;
        }
        elapsed_ms = run_test(inc_mutex, dec_mutex);
        pthread_mutex_destroy(&mutex);
    } else if (strcmp(sync_mechanism, "spinlock") == 0) {
        if (pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0) {
            perror("pthread_spin_init");
            return 1;
        }
        elapsed_ms = run_test(inc_spin, dec_spin);
        pthread_spin_destroy(&spinlock);
    } else {
        fprintf(stderr, "sync mechanism must be 'mutex' or 'spinlock'\n");
        return 1;
    }

    printf("execution time: %.2f ms, balance: %lld\n", elapsed_ms, balance);
    return 0;
}
