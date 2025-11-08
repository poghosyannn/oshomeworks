#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    const char *filename = "log.txt";
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char input[256];
    printf("Enter log message: ");
    fflush(stdout);

    ssize_t len = read(STDIN_FILENO, input, sizeof(input) - 1);
    if (len <= 0) {
        perror("read");
        close(fd);
        return 1;
    }

    input[len] = '\0';
    if (input[len - 1] == '\n') input[len - 1] = '\0';

    char logline[512];
    snprintf(logline, sizeof(logline), "PID=%d: %s\n", getpid(), input);

    if (write(fd, logline, strlen(logline)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    off_t pos = lseek(fd, 0, SEEK_CUR);
    printf("Current file offset: %ld\n", (long)pos);

    close(fd);
    return 0;
}

