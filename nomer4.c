#include <stdio.h>

int main() {
   
    int start, end, divisor;

    printf("Enter start value: ");
    scanf("%d", &start);

    printf("Enter end value: ");
    scanf("%d", &end);

    printf("Enter divisor value: ");
    scanf("%d", &divisor);

    
    for (int i = start; i <= end; i++) {
        
        if (i % divisor == 0) {
            printf("%d is divisible by %d\n", i, divisor);
        } else {
            printf("%d is not divisible by %d\n", i, divisor);
        }
    }

   
    int choice; 

    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Choice is 1\n");
            break;
        case 2:
            printf("Choice is 2\n");
            break;
        default:
            printf("Choice is not 1 or 2\n");
            break;
    }

    return 0;
}
