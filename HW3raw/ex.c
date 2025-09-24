#include <stdio.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid != 0) {
        // This block runs in the parent
        printf("Parent process ID: %d\n", getpid());
    } else {
        // This block runs in the child
        printf("Child process ID: %d\n", getpid());
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t first = fork();

    if (first == 0) {
        // First child branch
        printf("First child PID: %d\n", getpid());
    } else {
        // Parent waits for the first child
        wait(NULL);
        printf("Main parent PID: %d\n", getpid());

        // Create a second child
        pid_t second = fork();

        if (second == 0) {
            printf("Second child PID: %d\n", getpid());
        } else {
            // Explicitly wait for the second child using waitpid
            waitpid(second, NULL, 0);
            printf("Parent finished waiting for PID %d (self PID: %d)\n", second, getpid());
        }
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler_one(void) {
    printf("Handler one executed at exit!\n");
}

void handler_two(void) {
    printf("Handler two executed at exit!\n");
}

int main(void) {
    if (atexit(handler_one) != 0) {
        fprintf(stderr, "Failed to register handler_one\n");
    }

    if (atexit(handler_two) != 0) {
        fprintf(stderr, "Failed to register handler_two\n");
    }

    // Program exits here, triggering the handlers
    exit(EXIT_SUCCESS);

    // This line will never run
    printf("This will not print\n");
    return 0;
}