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
			// *(X+((D*i)+j))=drand(10, 1000);
		}
	}
	printBalk(X,N,D);

	// the rest will be inside the buildvp

	// Matrix T to keep the indeces of the elements
	int * T = malloc(N*sizeof(int));
	for (int i=0;i<N;i++){
		T[i] = i;
	}
	printf("-----\n");

	// Allocate memory for all the nodes of the tree.
	// vptree *Tree = malloc(N*sizeof(vptree));
	double * vp;
	for (int i=0; i<D;i++){
		int idx = (N-1)*D + i;
		vp[i] = X[idx];
	}
	vptree * Nodes = malloc(N*sizeof(vptree));
	

	
	// The VP will be the first element of every tree / sub-tree
	// vptree * nodeRoot = &Tree[T[N-1]];
	
	// newNode(nodeRoot, T[N-1], vp);
	vptree * rootNode;
	rootNode = buildvptree(Nodes, X, T, N, D);

	
	printf("DONE built tree \n");
	// int idx = getIDX(nodeRoot);
	// printf("\nthe idx of the root: %d\n", idx);
	printf("\n---------\n");
	printInorder(rootNode);
	printf("\n---------\n");
	// free(Nodes);
	free(T);
	free(X);
	return 0;
	// printf("now i will print it from the main\n");

	// vptree * No = buildvp(X, N, D);
	// printf("\nNow i will print the tree that i have from the getter\n");
	

	// for (int i=0;i<N;i++){
	// 	printf("| %d |", No[i]);
	// }
	// free(Nodes);
	// return 0;
}

