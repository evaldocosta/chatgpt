#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 1000

int main()
{
    int i, j, iter;
    float x_min = -2.0, x_max = 1.0, y_min = -1.5, y_max = 1.5;
    float dx = (x_max - x_min) / WIDTH;
    float dy = (y_max - y_min) / HEIGHT;

    // Allocate memory for image buffer
    unsigned char *buffer = (unsigned char *) malloc(WIDTH * HEIGHT * sizeof(unsigned char));

    // Compute Mandelbrot set using OpenACC directives
    #pragma acc data create(buffer[0:WIDTH*HEIGHT])
    #pragma acc parallel loop collapse(2) private(iter)
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            float x = x_min + j * dx;
            float y = y_min + i * dy;
            float zx = 0.0, zy = 0.0;
            iter = 0;
            while (iter < MAX_ITER && (zx * zx + zy * zy) < 4.0) {
                float new_zx = zx * zx - zy * zy + x;
                float new_zy = 2.0 * zx * zy + y;
                zx = new_zx;
                zy = new_zy;
                iter++;
            }
            buffer[i * WIDTH + j] = iter % 256;
        }
    }

    // Save image to file
    FILE *file = fopen("mandelbrot.pgm", "wb");
    fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(buffer, sizeof(unsigned char), WIDTH * HEIGHT, file);
    fclose(file);

    // Free memory
    free(buffer);

    return 0;
}