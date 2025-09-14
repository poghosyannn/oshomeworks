#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        printf("Child (PID=%d) running grep...\n", getpid());
        execl("/bin/grep", "grep", "main", "test.txt", NULL);
        exit(1);
    }
    else {
        wait(NULL); 
        printf("Parent process completed \n");
    }
    return 0;
}
