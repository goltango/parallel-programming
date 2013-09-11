#include <stdlib.h>
#include <stdio.h>
#define BUFFSIZE 1000
#define NUMTHREADS 4

/* Time in seconds from some point in the past */
double dwalltime();

int main(int argc, char *argv[])
{
	double **a;
	double **b;
	double **c;
	int i,j,k;
	int	tid, nthreads, chunk, num_threads, buffSize;
	double timetick;

    if (argc != 4)
    {
        printf("\nUsage: parallel_mm [buffSize] [num_threads] [chunk]\n");
        return 0;
    }

	buffSize = atoi(argv[1]);;
	num_threads = atoi(argv[2]);
	chunk = atoi(argv[3]);

	/*** alloc memory for a matrix ***/
	a = malloc(buffSize * sizeof(double *));
	for(i = 0; i < buffSize; i++)
	{
		a[i] = malloc(buffSize * sizeof(double));
	}
	/*** alloc memory for b matrix ***/
	b = malloc(buffSize * sizeof(double *));
	for(i = 0; i < buffSize; i++)
	{
		b[i] = malloc(buffSize * sizeof(double));
	}
	/*** alloc memory for c matrix ***/
	c = malloc(buffSize * sizeof(double *));
	for(i = 0; i < buffSize; i++)
	{
		c[i] = malloc(buffSize * sizeof(double));
	}
	/*** Set threads quantity ***/
	omp_set_num_threads(num_threads);

	/*** Set timetick before parallel execution ***/
    timetick = dwalltime();

	/*** Spawn a parallel region explicitly scoping all variables ***/
	#pragma omp parallel shared(a,b,c,nthreads,chunk,buffSize) private(tid,i,j,k)
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
		for (i=0; i<buffSize; i++)
			for (j=0; j<buffSize; j++)
				a[i][j]= 1.0;
		#pragma omp for schedule (static, chunk)
		for (i=0; i<buffSize; i++)
			for (j=0; j<buffSize; j++)
				b[i][j]= 1.0;
		#pragma omp for schedule (static, chunk)
		for (i=0; i<buffSize; i++)
			for (j=0; j<buffSize; j++)
				c[i][j]= 0;

		/*** Do matrix multiply sharing iterations on outer loop ***/
		/*** Display who does which iterations for demonstration purposes ***/
		printf("Thread %d starting matrix multiply...\n",tid);
		#pragma omp for schedule (static, chunk)
		for (i=0; i<buffSize; i++)
		{
			printf("Thread=%d did row=%d\n",tid,i);
			for(j=0; j<buffSize; j++)
                for (k=0; k<buffSize; k++)
                    c[i][j] += a[i][k] * b[k][j];
		}
	}   /*** End of parallel region ***/

	/*** Print results ***/
	printf("******************************************************\n");
	printf("Back from multiply in %f seconds\n", dwalltime() - timetick);
	printf("Example Matrix:\n");
	for (i=0; i<5; i++)
	{
		for (j=0; j<5; j++)
		printf("%.1f   ", c[i][j]);
		printf("\n");
	}
	printf("******************************************************\n");
	printf ("Done.\n");

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

