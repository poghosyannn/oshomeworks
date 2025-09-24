#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pid1 = fork();

    if (pid1 == 0) {
        exit(10);
    } else {
        int pid2 = fork();

        if (pid2 == 0) {
            exit(20);
        } else {
            int status;
            waitpid(pid1, &status, 0);
            printf("Child 1 exit status: %d\n", WEXITSTATUS(status));

            waitpid(pid2, &status, 0);
            printf("Child 2 exit status: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
