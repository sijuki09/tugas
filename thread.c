#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

typedef struct {
    int number;
} thread_data;

void* check_prime(void* arg) {
    thread_data* data = (thread_data*) arg;
    int number = data->number;
    if (is_prime(number)) {
        printf("%d prima\n", number);
    } else {
        printf("%d bukan prima\n", number);
    }
    free(data);  
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s num1 num2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = argc - 1;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_data* data = (thread_data*) malloc(sizeof(thread_data));
        if (data == NULL) {
            perror("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }
        data->number = atoi(argv[i + 1]);
        int rc = pthread_create(&threads[i], NULL, check_prime, (void*) data);
        if (rc) {
            fprintf(stderr, "Error: pthread_create, rc: %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
