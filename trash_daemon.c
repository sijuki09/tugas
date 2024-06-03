#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

// Fungsi untuk menghapus file dengan ekstensi .trash di direktori /tmp/trash
void delete_trash_files(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) {
            const char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".trash") == 0) {
                char full_path[256];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
                if (remove(full_path) == 0) {
                    printf("Deleted %s\n", full_path);
                } else {
                    perror("remove");
                }
            }
        }
    }

    closedir(dp);
}

int main() {
    pid_t pid, sid;

    // Fork the process
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    const char *trash_dir = "/tmp/trash";
    const char *stop_file = "/tmp/trash/stop.trash";

    while (1) {
        struct stat buffer;
        if (stat(stop_file, &buffer) == 0) {
            break; // Stop the daemon if stop.trash file exists
        }

        delete_trash_files(trash_dir);

        // Sleep for 30 seconds
        sleep(30);
    }

    exit(EXIT_SUCCESS);
}
