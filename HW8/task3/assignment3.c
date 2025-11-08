#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    char filepath[256];
    printf("Enter path: ");
    scanf("%255s", filepath);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    off_t filesize = lseek(fd, 0, SEEK_END);
    if (filesize == 0) {
        printf("\n");
        close(fd);
        return 0;
    }

    printf("Reversing file contents...\n");
    char byte;
    for (off_t pos = filesize - 1; pos >= 0; pos--) {
        lseek(fd, pos, SEEK_SET);
        if (read(fd, &byte, 1) != 1)
            break;
        write(STDOUT_FILENO, &byte, 1);
    }
    write(STDOUT_FILENO, "\n", 1);

    close(fd);
    return 0;
}

