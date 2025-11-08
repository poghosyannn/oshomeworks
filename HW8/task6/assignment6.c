#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const char *filename = "numbers.txt";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char line[8];
    for (int i = 1; i <= 10; i++) {
        snprintf(line, sizeof(line), "%d\n", i);
        if (write(fd, line, strlen(line)) < 0) {
            perror("write");
            close(fd);
            return 1;
        }
    }
    close(fd);

    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("reopen");
        return 1;
    }

    char buffer[1024];
    ssize_t len = read(fd, buffer, sizeof(buffer) - 1);
    if (len < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    buffer[len] = '\0';

    char *lines[20];
    int count = 0;
    char *token = strtok(buffer, "\n");
    while (token && count < 20) {
        lines[count++] = token;
        token = strtok(NULL, "\n");
    }

    int target = 4;
    if (target > 0 && target <= count)
        lines[target - 1] = "100";

    ftruncate(fd, 0);
    lseek(fd, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
        dprintf(fd, "%s\n", lines[i]);
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("verify open");
        return 1;
    }

    char out[256];
    ssize_t n;
    while ((n = read(fd, out, sizeof(out))) > 0)
        write(STDOUT_FILENO, out, n);

    close(fd);
    return 0;
}

