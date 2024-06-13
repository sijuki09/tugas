#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0;

pthread_mutex_t counter_mutex;

void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int num_threads = 4;
    pthread_t threads[num_threads];

    if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex\n");
        return 1;
    }

    for (int i = 0; i < num_threads; i++) {
        int rc = pthread_create(&threads[i], NULL, increment_counter, NULL);
        if (rc) {
            fprintf(stderr, "Error: pthread_create, rc: %d\n", rc);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&counter_mutex);

    printf("Final counter value: %d\n", counter);

    return 0;
}
