#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_FILES 3

void print_current_directory_info() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("current directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void print_first_n_files(int n) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    if ((dir = opendir(".")) != NULL) {
        printf("first %d files:\n", n);
        while ((entry = readdir(dir)) != NULL && count < n) {
            if (entry->d_type == DT_REG) {
                printf("%s\n", entry->d_name);
                count++;
            }
        }
        closedir(dir);
    } else {
        perror("opendir() error");
    }
}

int main() {
    print_current_directory_info();
    print_first_n_files(MAX_FILES);
    return 0;
}
