#include <stdlib.h>
#include <stdio.h>

int main(){
	double **arrayM;
	int nrows = 5,ncolumns=5;
	int i=0,j;

	arrayM = malloc(nrows * sizeof(double *));
	for(i = 0; i < nrows; i++)
    {
        arrayM[i] = malloc(ncolumns * sizeof(double));
    }

    for(i = 0; i < nrows; i++)
    {
        for(j = 0; j < ncolumns; j++)
        {
            arrayM[i][j] = 1.0;
        }

    }

    for(i = 0; i < nrows; i++)
    {
        printf("\n");
        for(j = 0; j < ncolumns; j++)
        {
            printf("%.1f ", arrayM[i][j]);
        }

    }

}
