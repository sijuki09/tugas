#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_prime(int n) {
    if (n <= 1) return 0; 
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return 0; 
    }
    return 1; 
}


void *check_prime(void *arg) {
    int num = *((int*)arg);
    if (is_prime(num)) {
        printf("%d prima\n", num);
    } else {
        printf("%d bukan prima\n", num);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    
    pthread_t threads[argc - 1];

    for (int i = 1; i < argc; ++i) {
        int num = atoi(argv[i]);
        pthread_create(&threads[i - 1], NULL, check_prime, (void*)&num);
    }

    
    for (int i = 0; i < argc - 1; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
