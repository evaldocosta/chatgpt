#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define LIMIT 1000000

bool is_prime(int n)
{
    // Check if n is a prime number
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    int sqrt_n = sqrt(n);
    for (int i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main()
{
    int count = 0;
    int *prime_numbers = NULL;

    // Allocate memory on GPU
    #pragma acc data create(prime_numbers[0:LIMIT])
    {
        prime_numbers = (int*) malloc(LIMIT * sizeof(int));

        // Find prime numbers
        #pragma acc parallel loop reduction(+:count)
        for (int i = 2; i < LIMIT; i++) {
            if (is_prime(i)) {
                prime_numbers[count++] = i;
            }
        }
    }

    // Print result
    printf("Prime numbers up to %d:\n", LIMIT);
    for (int i = 0; i < count; i++) {
        printf("%d ", prime_numbers[i]);
    }
    printf("\n");

    // Free memory on GPU
    free(prime_numbers);

    return 0;
}