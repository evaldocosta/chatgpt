#include <stdio.h>
#include <stdlib.h>
#include <openacc.h>

#define SIZE 8192

int main() {
int   i, j, k;
float temp;

float *a = (float*) malloc (sizeof(float)*SIZE*SIZE);
float *b = (float*) malloc (sizeof(float)*SIZE*SIZE);
float *c = (float*) malloc (sizeof(float)*SIZE*SIZE);

#pragma acc data copyin (a[0:SIZE*SIZE], b[0:SIZE*SIZE]), copy(c[0:SIZE*SIZE])
{
    #pragma acc parallel loop gang vector collapse(2)
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
          a[i*SIZE+j] = (float)i + j;
          b[i*SIZE+j] = (float)i - j;
          c[i*SIZE+j] = 0.0f;
      }
    } 
    #pragma acc parallel 
    #pragma acc loop tile(256,256) independent  
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
          temp  = 0.0; 
                 #pragma acc loop reduction(+:temp) 
                 for (k = 0; k < SIZE; ++k) {
                     temp += a[i*SIZE+k] + b[k*SIZE + j];
                 }
                 c[i*SIZE+j] = temp; 
      }   
    }
}

return 0;

}
