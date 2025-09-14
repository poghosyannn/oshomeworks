#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();   

    if (pid == 0) {  
        printf("Child with pid - %d running ls\n", getpid());

        execl("/bin/ls", "ls", "-l", NULL);
        exit(1);
    }
    else {
        wait(NULL);  // wait for child to finish
        printf("Parent process done \n");
    }

    return 0;
}
