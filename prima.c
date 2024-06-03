#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX 100

typedef struct {
    int num;
    bool is_prime;
} prime_check_t;

void *check_prime(void *arg) {
    prime_check_t *data = (prime_check_t *)arg;
    int num = data->num;
    data->is_prime = true;

    if (num <= 1) {
        data->is_prime = false;
    } else {
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                data->is_prime = false;
                break;
            }
        }
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <numbers>\n", argv[0]);
        return 1;
    }

    int num_threads = argc - 1;
    pthread_t threads[num_threads];
    prime_check_t prime_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        prime_data[i].num = atoi(argv[i + 1]);
        pthread_create(&threads[i], NULL, check_prime, &prime_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        printf("%d %s\n", prime_data[i].num, prime_data[i].is_prime ? "prima" : "bukan prima");
    }

    return 0;
}
