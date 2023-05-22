#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define X_RESN 800 /* x resolution */
#define Y_RESN 800 /* y resolution */

typedef struct complextype
{
  float real, imag;
} Compl;

void main(int argc, char *argv[]) {
/*   Window win;  */                       /* initialization for a window */
  unsigned int width, height,        /* window size */
      x, y,                          /* window position */
      border_width,                  /*border width in pixels */
      display_width, display_height, /* size of screen */
      screen;                        /* which screen */

  char *window_name = "Mandelbrot Set", *display_name = NULL;

  unsigned long valuemask = 0;

  FILE *fp, *fopen();
  char str[100];


  /* Mandlebrot variables */
  int i, j, k;
  Compl z, c;
  float lengthsq, temp;


  width = X_RESN;
  height = Y_RESN;

  x = 0;
  y = 0;

  border_width = 4;

  int counter = 0;

  #pragma acc parallel loop copy(counter)
  for (i = 0; i < X_RESN; i++)
    for (j = 0; j < Y_RESN; j++)
    {

      z.real = z.imag = 0.0;
      c.real = ((float)j - 400.0) / 200.0; /* scale factors for 800 x 800 window */
      c.imag = ((float)i - 400.0) / 200.0;
      k = 0;

      do
      { /* iterate for pixel color */

        temp = z.real * z.real - z.imag * z.imag + c.real;
        z.imag = 2.0 * z.real * z.imag + c.imag;
        z.real = temp;
        lengthsq = z.real * z.real + z.imag * z.imag;
        k++;
      } while (lengthsq < 4.0 && k < 100000);

      if (k == 100000){
        #pragma acc atomic update
          counter++;
      }
    }

  printf("Com tamanho de imagem X: %d e Y: %d \n", X_RESN, Y_RESN);
  
  printf("Counter %d\n", counter);

}
