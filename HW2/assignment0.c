#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
int main() 
{ 
    printf("Lvl0: Parent process id: %d\n", getpid());
    fork();
 
    printf("Lvl1: pId - %d and partent pId - %d \n", getpid(), getppid());
    sleep(1);
    fork();

    printf("Lvl2: pId - %d and partent pId - %d \n", getpid(), getppid());
    sleep(1);
    fork();

    printf("Lvl3: pId - %d and partent pId - %d \n", getpid(), getppid());
    return 0;
} 