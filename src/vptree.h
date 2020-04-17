#ifndef VPTREE_H
#define VPTREE_H

typedef struct vptree{
	struct vptree *inner;
	struct vptree *outer;
	double MD;
	double *VP;
	int idx;
}vptree ;


//! Print the elements of a double array
/*!

  \param low    The low limit for the double number
  \param high   The hugh limit for the double number
  \return The double random number
*/
double drand ( double low, double high );



//! Print the elements of a double array
/*!

  \param X      Input data points, stored as [n-by-d] array
  \param n      Number of data points (rows of X)
  \param d      Number of dimensions (columns of X)
*/
void printBalk(double *X, int n, int d);

//! Print the nodes of a tree with order
/*!

  \param root   The root node of the tree
*/
void printInorder(struct vptree* root);

//! Print the nodes of a tree using the accessors
/*!

  \param root   The root node of the tree
*/
void walkTheTree(struct vptree *root);

vptree * buildvptree(vptree *Nodes, double *X, int *T, int N, int D);

// ========== LIST OF ACCESSORS


//! Build vantage-point tree given input dataset X
/*!

  \param X      Input data points, stored as [n-by-d] array
  \param n      Number of data points (rows of X)
  \param d      Number of dimensions (columns of X)
  \return The vantage-point tree
*/
vptree * buildvp(double *X, int n, int d);

//! Return vantage-point subtree with points inside radius
/*!

  \param node   A vantage-point tree
  \return The vantage-point subtree
*/
vptree * getInner(vptree * T);

//! Return vantage-point subtree with points outside radius
/*!

  \param node   A vantage-point tree
  \return The vantage-point subtree
*/
vptree * getOuter(vptree * T);

//! Return median of distances to vantage point 
/*!

  \param node   A vantage-point tree
  \return The median distance
*/
double getMD(vptree * T);

//! Return the coordinates of the vantage point
/*!

  \param node   A vantage-point tree
  \return The coordinates [d-dimensional vector]
*/
double * getVP(vptree * T);

//! Return the index of the vantage point
/*!

  \param node   A vantage-point tree
  \return The index to the input vector of data points
*/
int getIDX(vptree * T);

#endif