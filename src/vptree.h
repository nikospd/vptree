#ifndef VPTREE_H
#define VPTREE_H

typedef struct vptree{
	struct vptree *inner;
	struct vptree *outer;
	double MD;
	double *VP;
	int idx;
}vptree ;


vptree * buildvptree(double *X, int *T, int N, int D);
void findVP(double *vp, double * X, int idx, int D);
void findDistances(double *distances, double *X, vptree *T, double *vp, int N, int D);
int findSubTrees(vptree *Tinner, vptree *Touter, vptree *T, double *distances, double MD, int N );
double drand ( double low, double high );
vptree* newNode(int data);

vptree * buildvp(double *X, int n, int d);

vptree * getInner(vptree * T);


vptree * getOuter(vptree * T);
double getMD(vptree * T);
double * getVP(vptree * T);
int getIDX(vptree * T);

#endif