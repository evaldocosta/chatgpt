#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

# pragma acc routine
double f(double x) {
double valor;
    valor = exp(x);
    return(valor);
}

int main(int argc, char *argv[]) {
double integral;
double a, b;
long i,n;
double h;

    a = 0.0;
    b = 1.0;
    n = 6000000;
    h = (b-a)/n;

    #pragma acc data copy(integral)
    integral = (f(a) + f(b))/2.0;

    #pragma acc parallel loop reduction(+:integral)
    for (i = 1; i < n-1; i++) {
        integral += f(a + i*h);
    }
    integral *= h ;
    // printf("Com n = %ld trapezoides, a estimativa \n", n);
    // printf("da integral de %f ate %f = %lf \n", a, b, integral);
    // printf("Tempo: \t %f \n", omp_get_wtime()-inicio);
    return(0);
}