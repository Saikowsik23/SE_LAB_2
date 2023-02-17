#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
void matrix_multiply(int block_size,int size,double **A,double **B,double **C){
	int i,j,k;
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
}
void transpose(int size,double **matrix, double **transposed){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
}
int main(int argc, char*argv[]) {
    int i,j,k;
	struct timeval tv1, tv2;
	struct timezone tz;
	double elapsed=0; 
	double total_elapsed=0;
	int size=atoi(argv[1]);
	int numTh=atoi(argv[2]);
	int power=atoi(argv[3]);
	int block_size=atoi(argv[4]);
	 double **A;
    double **tmp;
    double **res;
    double **trans;
	A=(double **)calloc(sizeof(double *), size);
	tmp=(double **)calloc(sizeof(double *), size);
	res=(double **)calloc(sizeof(double *), size);
    trans=(double **)calloc(sizeof(double *), size);
	for(i=0;i<size;i++){
		A[i]=(double *)calloc(sizeof(double), size);
		tmp[i]=(double *)calloc(sizeof(double), size);
		res[i]=(double *)calloc(sizeof(double), size);
        trans[i]=(double *)calloc(sizeof(double), size);
	}
	
	float randomNumber_A;
	srand(time(NULL));  // seed the random number generator
	for (int i= 0; i< size; i++){
        for(int j=0;j<size;j++){
  			randomNumber_A = (float)rand() / RAND_MAX;  // generate a random float between 0 and 1
            A[i][j] = randomNumber_A;
        }
    }
	//printf("Number of Threads: %d",omp_get_num_procs());
	omp_set_num_threads(numTh);
	//to check if the omp_set_num_threads(this part is correct or not)
	//printf("%d",numTh[c]);
	for(int avg=0;avg<5;avg++){
		gettimeofday(&tv1, &tz);
        transpose(size,A,trans);
	matrix_multiply(block_size,size,trans,trans,tmp);
    for(int i=2;i<power;i++){
        matrix_multiply(block_size,size,tmp,trans,res);
        for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            tmp[i][j]=res[i][j];
        }
    }
    }
	gettimeofday(&tv2, &tz);
	elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
	//remove the comment if you need to check if the part is correct
	printf("%4.2lf %d %d %d\n", elapsed,numTh,power,block_size);
	total_elapsed+=elapsed;
}
	printf("avg=%4.2lf %d %d %d\n", total_elapsed/5,numTh,power,block_size);

}
