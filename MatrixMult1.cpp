#include <stdio.h>
#include <ctime>
#define BUFFSIZE 800

int main(int argc, char *argv[]) {
    int i, j, k;
    int M = BUFFSIZE;
    int K = BUFFSIZE;
    int N = BUFFSIZE;
    float matA[BUFFSIZE][BUFFSIZE];
    float matB[BUFFSIZE][BUFFSIZE];
    float matC[BUFFSIZE][BUFFSIZE];

    for(i=0;i<M;i++){
        for(j=0;j<K;j++){
                matA[i][j]=1.0;
                matB[i][j]=1.0;
        }
    }
    printf("Multiplying %d x %d matrices\n", M, M);
    clock_t begin = clock();
    for(i=0;i<M;i++){
        for(j=0;j<K;j++){
            matC[i][j]=0.0;
            for(k=0;k<N;k++){
                matC[i][j]=matC[i][j]+matA[i][k]*matB[k][j];
            }
        }
    }
    clock_t end = clock();


    for(i=0;i<3;i++){
        printf("\n");
        for(j=0;j<3;j++){
                printf(" %.1f, ", matC[i][j]);
        }
    }


    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nExecutionTime: %fs \n", elapsed_secs);
}
