#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "vptree.h"

void printBalk(double *X, int N, int D);

void printInorder(struct vptree* node);

int main(){

	// create the dataset
	int N=20, D=2;
	double *X;
	X = malloc(N*D*sizeof(double));
	for (int i=0;i<N;i++){
		for (int j=0;j<D;j++){
			X[(D * i) + j] = drand(10, 100);
		}
	}
	printBalk(X,N,D);

	// the rest will be inside the buildvp

	// Vector T to keep the indeces of the elements
	int * T = malloc(N*sizeof(int));
	for (int i=0;i<N;i++){
		T[i] = i;
	}
	printf("-----\n");

	// Allocate memory for all the nodes of the tree.
	vptree * Nodes = malloc(N*sizeof(vptree));
	

	vptree * rootNode;
	rootNode = buildvptree(Nodes, X, T, N, D);

	
	printf("DONE built tree \n");
	printf("\n---------\n");
	printInorder(rootNode);
	printf("\n---------\n");
	free(Nodes);
	free(T);
	

	printf("\n---------\n");
	printf("Now i will build the same tree with the accessors \n");
	vptree * accRoot = buildvp(X, N, D);
	printf("Now i will print that tree");
	printf("\n---------\n");
	walkTheTree(accRoot);
	printf("\n---------\n");

	free(X);

	return 0;
}

