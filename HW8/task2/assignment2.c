#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    const char *fname = "data.txt";
    int fd;
    char buffer[64] = {0};

    printf("Creating %s and writing the alphabet\n", fname);
    fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    if (write(fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26) < 0) {
        perror("write failed");
        close(fd);
        return 1;
    }
    close(fd);

    printf("Reopening the file\n");
    fd = open(fname, O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    printf("Original size: %ld bytes\n", size);

    if (ftruncate(fd, 10) == -1) {
        perror("ftruncate failed");
        close(fd);
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    printf("After truncation: %ld bytes\n", size);

    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, sizeof(buffer) - 1);
    printf("Remaining data: %s\n", buffer);

    close(fd);
    return 0;
}

