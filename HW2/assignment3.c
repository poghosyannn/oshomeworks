#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        printf("Child process with pId %d: \n", getpid());
        execl("/bin/echo", "echo", "Hello from the child process", NULL);
        exit(1);
    }
    else {
        wait(NULL);  // wait for child to finish
        printf("Parent process done \n");
    }
    return 0;
}
