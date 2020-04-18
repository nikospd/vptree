/*
 * Sequential implementation of vantage point tree.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vptree.h"

#define limit 10000 //Limit for parallel

vptree * newNode(int idx, double* vp, double md) {
	vptree * p = malloc(sizeof(vptree));
	p->inner = NULL;
	p->outer = NULL;
	p->MD = md;
	p->VP = vp;
	p->idx = idx;
	return p;
}

// Helper function. Returns a double random number
double drand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

// Helper function. Prints a table o doubles. Used for debugging
void printBalk(double *X, int N, int D){
	for (int i=0;i<N;i++){
		for (int j=0;j<D;j++){
			printf(" %f || ",*(X+((D*i)+j)));
		}
		printf(" \n");
	}
	printf("-------\n");
}

// Helper funtion. Prints the tree with inner order. Used for debugging
void printInorder(struct vptree* root) 
{ 
  if (root == NULL) 
    return; 
  printInorder(root->inner);  
  printf("%d ", root->idx); 
  printInorder(root->outer); 
}

void findDistances(double *distances, double *X, int *T, double *vp, int N, int D){
	for (int i=0;i<(N-1);i++){
		double sum=0;
		int tmpidx = T[i];
		for (int j=0;j<D;j++){
			sum += pow((X[tmpidx*D+j] - vp[j]),2);
		}
		distances[i] = sqrt(sum);
	}
}

int findSubtrees(int *Tinner, int *Touter, int *T, double *distances, double MD, int N){
	int Ninner = 0;
	int Nouter = 0;
	for (int i=0;i<N-1;i++){
		if (distances[i] <= MD) {
			Tinner[Ninner] = T[i];
			Ninner += 1;
		}else {
			Touter[Nouter] = T[i];
			Nouter += 1;
		}
	}
	return Ninner;
}

int cmpfunc (const void * a, const void * b)
{
  return (*(double*)a > *(double*)b) ? 1 : (*(double*)a < *(double*)b) ? -1:0 ;
}

vptree * buildvptree(double * X, int * T,int  N, int D){

	if (N==1){
		int idx = T[0];
		double *vp = malloc(D*sizeof(double));
		for (int i=0; i<D;i++){
			vp[i] = X[idx*D + i];
		}
		vptree *node = newNode(idx, vp, 0);
		free(vp);
		return node;
	}else if(N == 2){ 
		int idx = T[1];
		int tmpidx = T[0];
		double *vp = malloc(D*sizeof(double));
		double MD, sum = 0;
		for (int i=0; i<D;i++){
			vp[i] = X[(idx-1)*D + i];
			sum += pow((X[idx*D + i] - X[tmpidx*D + i]), 2);
		}
		MD = sqrt(sum);
		vptree * node = newNode(idx, vp, MD);
		int *T2 = &T[N-2];
		node->inner = buildvptree(X, T2, N-1, D);
		free(vp);
		return node;
	}else{

		int idx = T[N-1];
		double *vp = malloc(D*sizeof(double));
		for (int i=0; i<D;i++){
			vp[i] = X[idx*D + i];
		}
		double * distances = malloc((N-1)*sizeof(double));
		findDistances(distances, X, T, vp, N, D);
		// making a template distances vector cause this implementation of the 
		// quick select, is messing with the structure of the original table
        
		double *quickdistances = malloc((N-1)*sizeof(double));
		for (int i=0;i<(N-1);i++){
			quickdistances[i]=distances[i];
		}
        // this is wrong. Along with the quick select we have to mess with the T
        // and not create new Tinner and Touter every time. 
		qsort(quickdistances, N-1, sizeof(double), cmpfunc);
		double MD = quickdistances[(N/2)-1];

		// Create the new node with the idx, vp and MD that just calculated 
		vptree * node = newNode(idx, vp, MD);
		
        /*!
        Find the new T vectors for the inner and outer Trees. Also the size of those trees
        The correct way is to mess with the T along with the vector of distances
        So we dont have to allocate and find new vectors of Tinner and Touter every time
        */
        
		int *Tinner = (int *)malloc(N*sizeof(int));
		int *Touter = (int *)malloc(N*sizeof(int));

		int Ninner = findSubtrees(Tinner, Touter, T, distances, MD, N);
		int Nouter = N - 1 - Ninner;
        /*
        This still works but its not the best solution
        */
		if(N*D>2*limit){
            #pragma omp parallel shared(node)
            {
                #pragma omp sections nowait
                {
                    #pragma omp section
                    node->inner = buildvptree(X, Tinner, Ninner, D);
                    #pragma omp section
		            node->outer = buildvptree(X, Touter, Nouter, D);
                }
            }
        }else{
            node->inner = buildvptree(X, Tinner, Ninner, D);
		    node->outer = buildvptree(X, Touter, Nouter, D);
        }
        

		// This node is part of the tree at the position of the idx
		free(distances);
		free(quickdistances);
		free(Tinner);
		free(Touter);
		free(vp);
		return node;
	}
}

vptree * buildvp(double *X, int n, int d){
	int * T = malloc(n*sizeof(int));
	for (int i=0;i<n;i++){
		T[i] = i;
	}
	vptree * rootNode;
	rootNode = buildvptree(X, T, n, d);
	free(T);
	return rootNode;
}
vptree * getInner(vptree * T){
	return T->inner;
}
vptree * getOuter(vptree * T){
	return T->outer;
}
double getMD(vptree *T){
	return T->MD;
}
double * getVP(vptree *T){
	return T->VP;
}
int getIDX(vptree *T){
	return T->idx;
}

void walkTheTree(vptree * root)
{

	if (root == NULL){return;}

	vptree * inner = getInner(root);
	vptree * outer = getOuter(root);
	
	walkTheTree(inner);  
  	int idx = getIDX(root);
	double MD = getMD(root);
	printf("idx: %d , MD: %f \n", idx, MD);
  	walkTheTree(outer); 
}
