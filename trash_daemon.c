#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define PATH "/tmp/trash"
#define EXTENSION ".trash"
#define STOP_FILE "stop.trash"

void delete_trash_files();
int file_exists(const char *filename);

int main() {
    // Buat child process dan tutup file descriptor stdin, stdout, stderr
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    } else if (pid > 0) {
        exit(0);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        delete_trash_files();
        sleep(30);
    }

    return 0;
}

void delete_trash_files() {
    DIR *directory = opendir(PATH);

    if (directory) {
        struct dirent *entry;
        struct stat file_stat;

        while ((entry = readdir(directory)) != NULL) {
            // Cek apakah file memiliki ekstensi .trash
            if (strstr(entry->d_name, EXTENSION) != NULL) {
                char path[100];
                snprintf(path, sizeof(path), "%s/%s", PATH, entry->d_name);

                if (stat(path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    // Cek apakah file stop.trash tidak ada
                    if (!file_exists(STOP_FILE)) {
                        if (remove(path) == 0) {
                            printf("Deleted file: %s\n", path);
                        } else {
                            fprintf(stderr, "Failed to delete file: %s\n", path);
                        }
                    }
                }
            }
        }

        closedir(directory);
    }
}

int file_exists(const char *filename) {
    int exists = 0;
    FILE *file = fopen(filename, "rb");

    if (file != NULL) {
        exists = 1;
        fclose(file);
    }

    return exists;
}
