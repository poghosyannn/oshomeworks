#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    const char *filename = "sparse.bin";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    write(fd, "START", 5);
    lseek(fd, 1024 * 1024, SEEK_CUR);

    write(fd, "END", 3);
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("reopen");
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    printf("Apparent file size: %ld bytes\n", (long)size);
    close(fd);

    return 0;
}

