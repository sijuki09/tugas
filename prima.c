#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Fungsi untuk mengecek apakah sebuah bilangan prima
bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    // Mengecek apakah ada argumen yang diberikan
    if (argc < 2) {
        printf("Gunakan: %s <angka1> <angka2> ...\n", argv[0]);
        return 1;
    }

    // Iterasi melalui semua argumen yang diberikan
    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]);
        if (isPrime(num)) {
            printf("%d prima\n", num);
        } else {
            printf("%d bukan prima\n", num);
        }
    }

    return 0;
}
