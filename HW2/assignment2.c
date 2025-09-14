#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        printf("Process 1 with pId %d: \n", getpid());
        execl("/bin/ls", "ls", "-l", NULL);
        exit(1);
    }

    pid2 = fork();
    if (pid2 == 0) {
        printf("Process 2 with pId %d: \n", getpid());
        execl("/bin/date", "date", NULL);
        exit(1);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent process done \n");
    return 0;
}
