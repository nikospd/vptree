#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/sort.h"
#include <time.h>
#include "vptree.h"

void printBalk(double *X, int N, int D);

// typedef struct vptree{
// 	struct vptree *inner;
// 	struct vptree *outer;
// 	double MD;
// 	double *VP;
// 	int idx;
// }vptree ;
void printInorder(struct vptree* node);

// struct vptree* newNode(int data) 
// { 
//   struct vptree* node = (struct foo*) 
//                        malloc(sizeof(struct foo)); 
//   node->x = data; 
//   node->fItem = NULL; 
   
//   return(node); 
// }

int main(){
	clock_t begin = clock();
	int N=1500000, D=2;
	double *X;
	X = (double *)malloc(N*D*sizeof(double));
	for (int i=0;i<N;i++){
		for (int j=0;j<D;j++){
			// *(X+((D*i)+j))=rand()%200;
			*(X+((D*i)+j))=drand(10, 1000);
		}
	}
	// printBalk(X,N,D);

	int * T = (int *)malloc(N*sizeof(int));
	for (int i=0;i<N;i++){
		*(T+i) = i;
	}
	printf("-----\n");
	vptree *node = newNode(0);
	node = buildvptree(X, T, N, D);
	int idx = getIDX(node);
	printf("\nthe idx of the root: %d\n", idx);
	// printf("\n---------\n");
	// printInorder(node);
	// printf("\n---------\n");

	// printf("now i will print it from the main\n");

	// vptree * T = buildvp(X, N, D);
	// printf("\nNow i will print the tree that i have from the getter\n");
	

	// for (int i=0;i<N;i++){
	// 	printf("| %d |", *(T+i));
	// }
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nexcecution time: %f\n", time_spent);
	return 0;
}

void printBalk(double *X, int N, int D){
	for (int i=0;i<N;i++){
		for (int j=0;j<D;j++){
			printf(" %f || ",*(X+((D*i)+j)));
		}
		printf(" \n");
	}
	printf("-------\n");
}


void printInorder(struct vptree* node) 
{ 
  if (node == NULL) 
    return; 
  printInorder(node->inner);  
  printf("%d ", node->idx); 
  printInorder(node->outer); 
}