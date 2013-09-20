#include <stdlib.h>
#include <stdio.h>

/* Time in seconds from some point in the past */
double dwalltime();
/* Show on screen CPU model */
void showCpuinfo();

int main(int argc, char *argv[])
{
	float **a;
	float **b;
	float **c;
	int i,j,k;
	int	tid, nthreads, chunk, num_threads, buffSize;
	double timetick1, timetick2;

    if (argc != 4)
    {
        printf("\nUsage: parallel_mm [buffSize] [chunk] [num_threads] \n");
        return 1;
    }

	buffSize = atoi(argv[1]);
	chunk = atoi(argv[2]);
	num_threads = atoi(argv[3]);

	/*** alloc memory for a matrix ***/
	a = malloc(buffSize * sizeof(float *));
	for(i = 0; i < buffSize; i++)
	{
		a[i] = malloc(buffSize * sizeof(float));
	}
	/*** alloc memory for b matrix ***/
	b = malloc(buffSize * sizeof(float *));
	for(i = 0; i < buffSize; i++)
	{
		b[i] = malloc(buffSize * sizeof(float));
	}
	/*** alloc memory for c matrix ***/
	c = malloc(buffSize * sizeof(float *));
	for(i = 0; i < buffSize; i++)
	{
		c[i] = malloc(buffSize * sizeof(float));
	}
	/*** Set threads quantity ***/
	omp_set_num_threads(num_threads);

	/*** Set timetick1 before parallel execution ***/
    timetick1 = dwalltime();

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
//		printf("Thread %d starting matrix multiply...\n",tid);
		#pragma omp for schedule (static, chunk)
		for (i=0; i<buffSize; i++)
		{
//			printf("Thread=%d did row=%d\n",tid,i);
			for(j=0; j<buffSize; j++)
                for (k=0; k<buffSize; k++)
                    c[i][j] += a[i][k] * b[k][j];
 //                   c[i][k] += a[i][j]*b[j][k];
		}
	}   /*** End of parallel region ***/

    /*** Set timetick2 after parallel execution ***/
    timetick2 = dwalltime() - timetick1;

	/*** Print results ***/
	printf("******************************************************\n");
	showCpuinfo();
	printf("Algorithm 1 (c[i][j] += a[i][k] * b[k][j])\n\n");
	printf("Back from multiply in %f seconds with %d thread(s)\n\n", timetick2, nthreads);
	printf(" ———————————————————\n");
	printf("| Thread   | Time   |\n");
	printf(" ——————————————————\n");
	printf("|    %d      %f |\n", nthreads, timetick2);
	printf("|___________________|\n");

	printf("Example Matrix:\n");
	for (i=0; i<5; i++)
	{
		for (j=0; j<5; j++)
		printf("%.1f   ", c[i][j]);
		printf("\n");
	}
	printf("******************************************************\n");
	printf ("Done.\n");
    return 0;
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

/*****************************************************************/
void showCpuinfo(){
	FILE *fd;
	int buffSize = 256;
	char cpuinfo1[buffSize+1];
	char cpuinfo2[buffSize+1];
	char cpuinfo3[buffSize+1];
	char str1 [20];
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];
	char str6 [20];
	FILE *in;
	extern FILE *popen();
	char buff[buffSize+1];

	fd = fopen("/proc/cpuinfo","r");
	fgets(cpuinfo1, buffSize+1, fd);
	fgets(cpuinfo1, buffSize+1, fd);
	fgets(cpuinfo2, buffSize+1, fd);
	fgets(cpuinfo2, buffSize+1, fd);
	fgets(cpuinfo2, buffSize+1, fd);
	fclose(fd);
	fflush (fd);

	/*** Filter type and print it on screen ***/
	sscanf(cpuinfo1,"%*s %*s %s ", str1);
	printf("CPU Type: %s \n",str1);

	/*** Filter model, copy it to str2 and print it on screen ***/
	sscanf(cpuinfo2,"%*s %*s %*s %s %s %s %s %s %s", str1, str2, str3, str4, str5, str6);
	printf("CPU Model: %s %s %s\n",str1, str2, str3, str5, str6);

	/** Shows CPU cores**/
	in = popen("cat /proc/cpuinfo | grep processor | wc -l", "r");
	printf("CPU cores: ");
	while(fgets(buff, sizeof(buff), in)!=NULL){
		printf("%s", buff);
	}
	pclose(in);
}
