#include <stdio.h>
#include <stdlib.h>
#include <openacc.h>

#define N 1024

int main() {
    int i, j, k;

    // Allocate memory for matrices
    float *A = (float*)malloc(N * N * sizeof(float));
    float *B = (float*)malloc(N * N * sizeof(float));
    float *C = (float*)malloc(N * N * sizeof(float));

    // Initialize matrices
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i * N + j] = i + j;
            B[i * N + j] = i - j;
        }
    }

    // Multiply matrices using OpenACC
    #pragma acc data copyin(A[0:N*N], B[0:N*N]), create(C[0:N*N])
    {
        #pragma acc kernels
        {
            #pragma acc loop independent collapse(2)
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++) {
                    float temp = 0.0f;
                    for (k = 0; k < N; k++) {
                        temp += A[i * N + k] * B[k * N + j];
                    }
                    C[i * N + j] = temp;
                }
            }
        }
    }

    // Print result
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f ", C[i * N + j]);
        }
        printf("\n");
    }

    // Free memory
    free(A);
    free(B);
    free(C);

    return 0;
}