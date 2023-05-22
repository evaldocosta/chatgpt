#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define N 1000
#define TOLERANCE 1e-6

int main() {
    int i, j, k;
    double diff, sum, norm;
    double A[N][N], b[N], x[N], x_new[N];

    // Initialize A, b, and x
    for (i = 0; i < N; i++) {
        b[i] = 1.0;
        x[i] = 0.0;
        for (j = 0; j < N; j++) {
            if (i == j) {
                A[i][j] = 2.0;
            } else {
                A[i][j] = 1.0;
            }
        }
    }

    // Perform Jacobi iteration using OpenACC
    #pragma acc data copyin(A[0:N][0:N],b[0:N]), copy(x[0:N]), create(x_new[0:N])
    {
        for (k = 0; k < 100; k++) {
            #pragma acc kernels loop independent private(sum) collapse(2)
            for (i = 1; i < N-1; i++) {
                for (j = 1; j < N-1; j++) {
                    sum = A[i][j-1] * x[j-1] + A[i][j+1] * x[j+1] +
                          A[i-1][j] * x[j] + A[i+1][j] * x[j] - A[i][j] * x[j] - b[i];
                    x_new[j] = sum / A[i][j];
                }
            }
            diff = 0.0;
            norm = 0.0;
            #pragma acc parallel loop reduction(max:diff,norm)
            for (i = 0; i < N; i++) {
                diff = fmax(diff, fabs(x_new[i] - x[i]));
                norm = fmax(norm, fabs(x_new[i]));
            }
            if (norm != 0.0 && diff / norm < TOLERANCE) {
                break;
            }
            #pragma acc parallel loop
            for (i = 0; i < N; i++) {
                x[i] = x_new[i];
            }
        }
    }

    // Print result
    printf("Solution after %d iterations:\n", k);
    for (i = 0; i < N; i++) {
        printf("%f\n", x[i]);
    }

    return 0;
}