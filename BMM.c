#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
int main(int argc, char*argv[]) {
    int i,j,k;
	struct timeval tv1, tv2;
	struct timezone tz;
	double elapsed=0; 
	int size;
	printf("enter the matrix size:");
	scanf("%d",&size);
	int numTh[9]={1,2,4,6,8,10,12,14,16};
	//printf("Size is %d",N);
	 double **A;
    double **B;
    double **C;
	A=(double **)calloc(sizeof(double *), size);
	B=(double **)calloc(sizeof(double *), size);
	C=(double **)calloc(sizeof(double *), size);
	for(i=0;i<size;i++)
	{
		A[i]=(double *)calloc(sizeof(double), size);
		B[i]=(double *)calloc(sizeof(double), size);
		C[i]=(double *)calloc(sizeof(double), size);
	}
	
	float randomNumber_A,randomNumber_B;
	srand(time(NULL));  // seed the random number generator
	for (int i= 0; i< size; i++){
        for(int j=0;j<size;j++){
  			randomNumber_A = (float)rand() / RAND_MAX;  // generate a random float between 0 and 1
			randomNumber_B = (float)rand() / RAND_MAX; 
            A[i][j] = randomNumber_A;
		    B[i][j] = randomNumber_B;
        }
    }
	//printf("Number of Threads: %d",omp_get_num_procs());
	FILE *fp=NULL;
	fp=fopen("BMM.txt","w");
	int block_size=4; 
for(int c=0;c<9;c++){
	omp_set_num_threads(numTh[c]);
	//to check if the omp_set_num_threads(this part is correct or not)
	//printf("%d",numTh[c]);
	for(int avg=0;avg<5;avg++){
		gettimeofday(&tv1, &tz);
	 #pragma omp parallel for private(i,j,k) shared(A,B,C)
for (int i = 0; i < size; i += block_size) {
    for (int j = 0; j < size; j += block_size) {
        for (int k = 0; k < size; k += block_size) {
            for (int ii = i; ii < i + block_size; ++ii) {
                for (int jj = j; jj < j + block_size; ++jj) {
                    for (int kk = k; kk < k + block_size; ++kk) {
                        C[ii][jj] += A[ii][kk] * B[kk][jj];
                    }
                }
            }
        }
    }
}
	gettimeofday(&tv2, &tz);
	elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
	//remove the comment if you need to check if the part is correct
	//printf("elapsed time = %4.2lf seconds for the thread %d.\n", elapsed,numTh[c]);
}
	printf(" average elapsed time = %4.2lf seconds for the thread %d.\n", elapsed/5,numTh[c]);
	fprintf(fp,"%d\t %lf\n",numTh[c],elapsed/5);
	elapsed=0;
}
}
