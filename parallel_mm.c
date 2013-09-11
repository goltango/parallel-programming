#include <stdlib.h>
#include <stdio.h>

#define BUFFSIZE 500
int main(int argc, char *argv[])
{
	double **a;
	double **b;
	double **c;
	int nrows = BUFFSIZE,ncolumns = BUFFSIZE;
	int i,j,k;
	int	tid, nthreads, chunk;
	chunk = 10;
    //reserva de memoria de a
	a = malloc(nrows * sizeof(double *));
	for(i = 0; i < nrows; i++)
    {
        a[i] = malloc(ncolumns * sizeof(double));
    }
    //reserva de memoria de b
	b = malloc(nrows * sizeof(double *));
	for(i = 0; i < nrows; i++)
    {
        b[i] = malloc(ncolumns * sizeof(double));
    }
    //reserva de memoria de c
	c = malloc(nrows * sizeof(double *));
	for(i = 0; i < nrows; i++)
    {
        c[i] = malloc(ncolumns * sizeof(double));
    }

    /*** Spawn a parallel region explicitly scoping all variables ***/
    #pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
      {
      tid = omp_get_thread_num();
      if (tid == 0)
        {
        nthreads = omp_get_num_threads();
        printf("Starting matrix multiple example with %d threads\n",nthreads);
        printf("Initializing matrices...\n");
        }
      /*** Initialize matrices ***/
      #pragma omp for schedule (static, chunk)
      for (i=0; i<BUFFSIZE; i++)
        for (j=0; j<BUFFSIZE; j++)
          a[i][j]= 1.0;
      #pragma omp for schedule (static, chunk)
      for (i=0; i<BUFFSIZE; i++)
        for (j=0; j<BUFFSIZE; j++)
          b[i][j]= 1.0;
      #pragma omp for schedule (static, chunk)
      for (i=0; i<BUFFSIZE; i++)
        for (j=0; j<BUFFSIZE; j++)
          c[i][j]= 0;

      /*** Do matrix multiply sharing iterations on outer loop ***/
      /*** Display who does which iterations for demonstration purposes ***/
      printf("Thread %d starting matrix multiply...\n",tid);
      #pragma omp for schedule (static, chunk)
      for (i=0; i<BUFFSIZE; i++)
        {
        printf("Thread=%d did row=%d\n",tid,i);
        for(j=0; j<BUFFSIZE; j++)
          for (k=0; k<BUFFSIZE; k++)
            c[i][j] += a[i][k] * b[k][j];
        }
      }   /*** End of parallel region ***/

    /*** Print results ***/
    printf("******************************************************\n");
    printf("Result Matrix:\n");
    for (i=0; i<BUFFSIZE; i++)
      {
      for (j=0; j<BUFFSIZE; j++)
        printf("%.1f   ", c[i][j]);
      printf("\n");
      }
    printf("******************************************************\n");
    printf ("Done.\n");

}
