#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSZ 32768

static ssize_t writen_all(int fd, const void *buf, size_t n) {
    const char *p = (const char *)buf;
    size_t left = n;
    while (left > 0) {
        ssize_t k = write(fd, p, left);
        if (k < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += k;
        left -= (size_t)k;
    }
    return (ssize_t)n;
}

static void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len && s[len-1] == '\n') s[len-1] = '\0';
}

int main(void) {
    char src[512], dst[512];

    printf("Source file path: ");
    if (!fgets(src, sizeof src, stdin)) { perror("read src path"); return 1; }
    strip_newline(src);

    printf("Destination file path: ");
    if (!fgets(dst, sizeof dst, stdin)) { perror("read dst path"); return 1; }
    strip_newline(dst);

    int in = open(src, O_RDONLY);
    if (in < 0) { perror("open src"); return 1; }

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { perror("open dst"); close(in); return 1; }

    char buf[BUFSZ];
    ssize_t nread;
    size_t total = 0;

    while (1) {
        nread = read(in, buf, sizeof buf);
        if (nread == 0) break;           
        if (nread < 0) {
            if (errno == EINTR) continue;
            perror("read");
            close(in); close(out);
            return 1;
        }
        if (writen_all(out, buf, (size_t)nread) < 0) {
            perror("write");
            close(in); close(out);
            return 1;
        }
        total += (size_t)nread;
    }

    printf("Bytes copied: %zu\n", total);
    close(in);
    close(out);
    return 0;
}
