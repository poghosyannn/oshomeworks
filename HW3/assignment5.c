#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        printf("Child pid: %d\n", getpid());
    } else {
        sleep(30); // child becomes zombie until wait() is called
        wait(NULL);
        printf("Parent pid: %d\n", getpid());
    }
    return 0;
}
