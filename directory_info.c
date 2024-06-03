#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t pid;

    // Buat pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork proses
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proses anak
        // Tutup ujung baca pipe
        close(pipe_fd[0]);

        // Dapatkan path direktori aktif
        char cwd[BUFFER_SIZE];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        // Cetak path ke pipe
        if (write(pipe_fd[1], cwd, strlen(cwd)) != strlen(cwd)) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Buka direktori aktif
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        // Cetak tiga file pertama ke pipe
        for (int i = 0; i < 3; ++i) {
            struct dirent *entry = readdir(dir);
            if (entry != NULL) {
                if (write(pipe_fd[1], entry->d_name, strlen(entry->d_name)) != strlen(entry->d_name)) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // Tutup direktori
        closedir(dir);

        // Tutup ujung tulis pipe
        close(pipe_fd[1]);
    } else {  // Proses induk
        // Tutup ujung tulis pipe
        close(pipe_fd[1]);

        // Baca path dari pipe
        char path[BUFFER_SIZE];
        ssize_t bytes_read = read(pipe_fd[0], path, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        path[bytes_read] = '\0';

        // Cetak path
        printf("Direktori aktif: %s\n", path);

        // Baca dan cetak tiga file pertama dari pipe
        char filename[BUFFER_SIZE];
        for (int i = 0; i < 3; ++i) {
            ssize_t bytes_read = read(pipe_fd[0], filename, BUFFER_SIZE);
            if (bytes_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            filename[bytes_read] = '\0';
            printf("File %d: %s\n", i + 1, filename);
        }

        // Tutup ujung baca pipe
        close(pipe_fd[0]);
    }

    return 0;
}
