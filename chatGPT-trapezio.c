#include <stdio.h>

#define N 1000000  // Number of intervals

float f(float x) {
    // Define the function here
    return x * x;
}

int main() {
    float a = 0.0;          // Lower limit
    float b = 1.0;          // Upper limit
    float h = (b - a) / N;  // Step size

    float sum = 0.0;

    #pragma acc data copy(sum)
    {
        // Compute the sum of the function values
        #pragma acc parallel loop reduction(+:sum)
        for (int i = 1; i < N; i++) {
            float x = a + i * h;
            sum += f(x);
        }
    }

    float integral = h * (0.5 * (f(a) + f(b)) + sum);

    printf("Integral: %f\n", integral);

    return 0;
}