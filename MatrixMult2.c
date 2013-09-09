#include<stdio.h>
#include<stdlib.h>

/* Init square matrix with randm values */
void initrandmat(double mat[], int n);

/* Multiply square matrices */
void matmul(double *a, double *b,double *c, int n);

/* Time in seconds from some point in the past */
double dwalltime();

/************** MAIN *************/
main(int argc, char *argv[])
{
  double *a, *b, *c;
  int n;

  double timetick;

  /* Check command line parameters */
  if ((argc != 2) || ((n = atoi(argv[1])) <= 0) )
  {
    printf("\nUsage: %s n\n  n: matrix order (nxn X nxn)\n", argv[0]);
    exit(1);
  }

  /* Getting memory */
  a = (double *) malloc(n*n*sizeof(double));
  b = (double *) malloc(n*n*sizeof(double));
  c = (double *) malloc(n*n*sizeof(double));

  /* Init matrix operands */
  initrandmat(a, n);
  initrandmat(b, n);

  printf("Multiplying %d x %d matrices\n", n, n);

  timetick = dwalltime();

  matmul(a, b, c, n);

  printf("Back from multiply in %f seconds\n", dwalltime() - timetick);
}

/*****************************************************************/

/* Init square matrix with randm values */
void initrandmat(double mat[], int n)
{
  int i, j;      /* Indices */

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      mat[i*n + j] = (double) rand();
    }
  }
}

/*****************************************************************/

/* Multiply square matrices */
void matmul(double *a, double *b,double *c, int n)
{
  int i, j, k;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      c[i*n + j] = 0.0;
      for  (k = 0; k < n; k++)
      {
        c[i*n + j] += a[i*n + k] * b[k*n + j];
      }
    }
  }
}

/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
