/*
 * Sequential implementation of vantage point tree.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vptree.h"


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
void swap_double(double *a, double *b)
{
    double t = *a;
    *a = *b;
    *b = t;
}
void swap_int(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
int partition(int *T,double *distances, int l, int N) 
{ 
    double x = distances[N];
    int i = l; 
    for (int j = l; j <= N - 1; j++) { 
        if (distances[j] <= x) { 
            swap_double(&distances[i], &distances[j]); 
            swap_int(&T[i],&T[j]);
            i++; 
        } 
    } 
    swap_double(&distances[i], &distances[N]); 
    swap_int(&T[i],&T[N]);
    return i; 
} 
double qselect(int *T,double *distances, int l, int N, int k) 
{ 

    if (k > 0 && k <= N - l + 1) { 
  
        int index = partition(T,distances, l, N); 

        if (index - l == k - 1) 
            return distances[index]; //it is equal return median value 
  
        if (index - l > k - 1)  
            return qselect(T,distances, l, index - 1, k); //if index greater than k set the boundaries to the left side of the array
  
        return qselect(T,distances, index + 1, N,(k - index + l - 1)); //set the boundaries to the right side and set the k correct
    } 
  
    return -1; 
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
		node->inner = buildvptree(X, &T[N-2], N-1, D);
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
		int mdidx = floor(N/2);
		double MD = qselect(T,distances,0,N-2,mdidx);
		vptree * node = newNode(idx, vp, MD);
		
		node ->inner = buildvptree(X, T, mdidx, D);
		node ->outer = buildvptree(X, &T[mdidx], N-1-mdidx, D);

		// This node is part of the tree at the position of the idx
		free(distances);

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
