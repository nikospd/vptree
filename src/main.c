#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "vptree.h"
#include <omp.h>

void printBalk(double *X, int N, int D);

void printInorder(struct vptree* node);

int main(){
	clock_t begin = clock();
	// create the dataset
	int N=10000000, D=4;
	double *X;
	X = malloc(N*D*sizeof(double));
	for (int i=0;i<N;i++){
		for (int j=0;j<D;j++){
			X[(D * i) + j] = drand(10, 100);
		}
	}
	// printBalk(X,N,D);
	
	
	vptree * accRoot = buildvp(X, N, D);
	// printf("\n---------\n");
	// printInorder(accRoot);
	// printf("\n---------\n");

	free(X);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nexcecution time: %f\n", time_spent);
	return 0;
}

