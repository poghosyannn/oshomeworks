#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pid1 = fork();

    if (pid1 == 0) {
        printf("First child process: %d\n", getpid());
        exit(0);
    } else {
        wait(NULL);
        printf("Parent after first child: %d\n", getpid());

        int pid2 = fork();
        if (pid2 == 0) {
            printf("Second child process: %d\n", getpid());
            exit(5);
        } else {
            int status;
            waitpid(pid2, &status, 0);
            printf("Parent waited for second child %d, exit status %d\n", pid2, WEXITSTATUS(status));
        }
    }
    return 0;
}
