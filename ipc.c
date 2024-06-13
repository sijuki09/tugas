#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHMSZ 27

int main() {
    int shmid;
    key_t key;
    char *shm, *s;

    // Menghasilkan key
    key = 5678;

    // Membuat shared memory
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Melekatkan shared memory
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Menuliskan pesan ke shared memory
    strcpy(shm, "Hello, World!");

    // Menunggu proses lain membaca shared memory
    while (*shm != '*')
        sleep(1);

    printf("Data yang dibaca dari shared memory: %s\n", shm);

    // Melepas shared memory
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Menghapus shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
