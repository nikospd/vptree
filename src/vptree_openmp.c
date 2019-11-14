/*
 * Sequential implementation of vantage point tree.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/sort.h"
#include <omp.h>

typedef struct vptree{
	struct vptree *inner;
	struct vptree *outer;
	double MD;
	double *VP;
	int idx;
}vptree ;

vptree* newNode(int data){
	vptree* node = (vptree*)malloc(sizeof(vptree)); 
  	node->inner = NULL; 
	node->outer = NULL; 
	node->MD = 0;
	node->VP = NULL;
  	node->idx = data; 
	return(node); 
}

double drand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}


void findVP(double *vp, double * X, int idx, int D){
	// printf("the idx of the vp: %d\n", idx);
	for (int i=0;i<D;i++){
		*(vp+i) = *(X+idx*D+i);
	}
	// printf("the vp is: \n");
	// for (int i=0;i<D;i++){
	// 	printf(" %f ", *(vp+i));
	// }
	// printf("\n");
	// printf("-------\n");
	
}

void findDistances(double *distances, double *X, int *T, double *vp, int N, int D){
	#pragma omp for 
	for (int i=0;i<(N-1);i++){
		// printf("\ncalculating distance for point: %d\n", i);
		double sum=0;
		for (int j=0;j<D;j++){
			// printf("\ncalculating distance for dimension: %d\n", j);
			int tmpidx = *(T+i);
			sum += pow((*(X+tmpidx*D+j) - *(vp+j)),2);
			
		}
		*(distances + i) = sqrt(sum);
	}
}

int findSubTrees(int *Tinner, int *Touter, int *T, double *distances, double MD, int N ){
	int numInner = 0;
	int numOuter = 0;
	for (int i=0;i<N-1;i++){
		if (*(distances+i) > MD){
			*(Touter+numOuter) = *(T+i);
			numOuter += 1;
		}else{
			*(Tinner+numInner) = *(T+i);
			numInner += 1;
		}
	}
	return numInner;
}


vptree * buildvptree(double *X, int *T, int N, int D){
	// printf("\nnumber of thread for inner: %d\n",omp_get_thread_num());
	if (N == 1){
		// printf("\nhere for 1\n");
		int idx = *(T+N-1);
		vptree *node = newNode(idx);
		// node->idx=idx;
		node->inner=NULL;
		node->outer=NULL;
		double *vp = (double *)malloc(D*sizeof(double));
		findVP(vp, X, idx, D);
		node->VP=vp;
		// nodeptr=&node;
		// return nodeptr;
		return node;
		
		
	}else if (N == 2){
		// printf("\nhere for 2\n");
		int idx = *(T+N-1);
		vptree *node = newNode(idx);
		// node->idx=idx;
		node->inner=NULL;
		node->outer=NULL;
		double *vp = (double *)malloc(D*sizeof(double));
		findVP(vp, X, idx, D);
		node->VP=vp;
		int *T2 = (int *)malloc(N*sizeof(int));
		*T2 = *(T+N-2);
		vptree *node2;
		node2 = buildvptree(X, T2, N-1, D);
		node->inner=node2;
		// nodeptr=&node;
		return node;
	}


	// Find the Vantage Point
	int idx = *(T+N-1);
	vptree *node = newNode(idx);
	// node->idx=idx;
	// printf("\nthe idx of the vp inside the build: %d", node->idx);
	node->inner=NULL;
	node->outer=NULL;
	double *vp = (double *)malloc(D*sizeof(double));
	findVP(vp, X, idx, D);
	node->VP=vp;
	// Find the distances vector
	double *distances = (double *)malloc((N-1)*sizeof(double));
	findDistances(distances, X, T, vp, N, D);

	// making a template distances vector cause this implementation of the 
	// quick select, is messing with the structure of the original table
	double *quickdistances = (double *)malloc((N-1)*sizeof(double));

	for (int i=0;i<(N-1);i++){
		*(quickdistances+i)=*(distances+i);
	}

	// double MD = quickselect(quickdistances, 0, N-1 , ll);
	double MD;
	quickSort(quickdistances, 0, N-2);
	
	// printf("\nprint the SORTED distances\n");
    // for (int i=0;i<(N-1);i++){
    //     printf(" %f \n", *(quickdistances+i));
    // }

	MD = *(quickdistances+(N/2)-1);
	node->MD=MD;
	// I dont need this vector any more. I found the median distance
	free(quickdistances);
	
	// Alocate memory for the subtrees
	int *Tinner = (int *)malloc(N*sizeof(int));
	int *Touter = (int *)malloc(N*sizeof(int));

	// Find the subtrees
	int numInner = findSubTrees(Tinner, Touter, T, distances, MD, N);
	int numOuter = N-numInner-1;

	// printf("i will print the inner\n");
	// for (int i=0;i<numInner;i++){
	// 	printf("| %d |", *(Tinner+i));
	// }

	// printf("\ni will print the outer\n");
	// for (int i=0;i<numOuter;i++){
	// 	printf("| %d |", *(Touter+i));
	// }
	// printf("\n");
	// ----Fill the structure with Inner|VP|Outer
	for (int i=0;i<numInner;i++){
		*(T+i) = *(Tinner+i);
	} 
	*(T+numInner) = idx;
	for (int i=0;i<numOuter;i++){
		*(T+numInner+1+i) = *(Touter+i);
	}
	// ----Recursion----
	vptree *inner = newNode(0), *outer = newNode (0);
	// #pragma omp parallel sections
	// {

	// 	#pragma omp section 
	// 	{

	// 		#pragma omp shared(inner)
	// 		// printf("\nid of the inner: %d\n",omp_get_thread_num());
	inner = buildvptree(X, Tinner, numInner, D);
	// 	}
	// // printf("\nthe root idx is: %d and the inner idx is: %d\n", node->idx, inner->idx);
	// 	#pragma omp section 
	// 	{

	// 		#pragma omp shared(outer)
	// 		// printf("\nid of the outer: %d\n",omp_get_thread_num());
	outer = buildvptree(X, Touter, numOuter, D);
		// }
	// 	}
	// }
		

	node->inner=inner;
	node->outer=outer;
	// nodeptr = &node;
	return node;
	
}

vptree * buildvp(double *X, int n, int d){
	vptree * node = newNode(0);
	int * T;
	T = (int *)malloc(n*sizeof(int));
	for (int i=0;i<n;i++){
		*(T+i) = i;
	}
	node = buildvptree(X, T, n, d);
	return node;
}

int getIDX(vptree * T){
	return T->idx;
}

double * getVP(vptree * T){
	return T->VP;
}

double getMD(vptree * T){
	return T->MD;
}

vptree * getOuter(vptree * T){
	return T->outer;
}

vptree * getInner(vptree * T){
	return T->inner;
}
