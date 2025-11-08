#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF 4096

int main(void) {
    char path1[256], path2[256];

    printf("First file: ");
    if (scanf("%255s", path1) != 1) {
        fprintf(stderr, "Failed to read first path\n");
        return 1;
    }

    printf("Second file: ");
    if (scanf("%255s", path2) != 1) {
        fprintf(stderr, "Failed to read second path\n");
        return 1;
    }

    int fd1 = open(path1, O_RDONLY);
    int fd2 = open(path2, O_RDONLY);
    if (fd1 < 0 || fd2 < 0) {
        perror("open");
        if (fd1 >= 0) close(fd1);
        if (fd2 >= 0) close(fd2);
        return 1;
    }

    unsigned char b1[BUF], b2[BUF];
    unsigned long long offset = 0;

    for (;;) {
        ssize_t n1 = read(fd1, b1, sizeof b1);
        ssize_t n2 = read(fd2, b2, sizeof b2);
        if (n1 < 0 || n2 < 0) {
            perror("read");
            close(fd1);
            close(fd2);
            return 1;
        }

        ssize_t m = (n1 < n2) ? n1 : n2;
        for (ssize_t i = 0; i < m; i++) {
            if (b1[i] != b2[i]) {
                printf("Files differ at byte %llu\n", offset + (unsigned long long)i);
                close(fd1);
                close(fd2);
                return 1;
            }
        }

        if (n1 != n2) {
            printf("Files differ at byte %llu (EOF of shorter file)\n",
                   offset + (unsigned long long)m);
            close(fd1);
            close(fd2);
            return 1;
        }

        if (n1 == 0 && n2 == 0) {
            break;  // both EOF
        }

        offset += (unsigned long long)n1;
    }

    printf("Files are identical\n");
    close(fd1);
    close(fd2);
    return 0;
}

