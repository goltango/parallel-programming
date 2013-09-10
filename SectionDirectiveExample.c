#include <omp.h>
#include <stdio.h>
#define N     1000

main (){

int i, tid;
float a[N], b[N], c[N], d[N];

/* Some initializations */
for (i=0; i < N; i++) {
  a[i] = i * 1.5;
  b[i] = i + 22.35;
  }

#pragma omp parallel shared(a,b,c,d) private(tid,i)
  {

  #pragma omp sections nowait
    {

    #pragma omp section
    for (i=0; i < N; i++){
      	c[i] = a[i] + b[i];
	tid = omp_get_thread_num();
	printf("From Thread %d: d[%.1f]\n", tid, c[i]);
	}
    #pragma omp section
    for (i=0; i < N; i++){
      	d[i] = a[i] * b[i];
	tid = omp_get_thread_num();
	printf("From Thread %d: d[%.1f]\n",tid, d[i]);
	}
    }  /* end of sections */

  }  /* end of parallel section */

}
