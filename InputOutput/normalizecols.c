/* normalize the columns of a matrix
Syntax:  B = normalizecol(A)
	or:  B = normalizecol(A, p)
The columns of matrix A are normalized so that norm(B(:,n), p) = 1. */
#include <math.h>
#include "mex.h"

#define IS_REAL_2D_FULL_DOUBLE(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P) && mxIsDouble(P))
#define IS_REAL_SCALAR(P) (IS_REAL_2D_FULL_DOUBLE(P) && mxGetNumberOfDimensions(P) == 1)

/*
 * mexFunction is the user defined C routine which is called upon invocation of a mex function
 * nlhs : number of expected outputs;
 * *plhs[] : array of pointers to output arguments
 * nrhs : number of inputs;
 * *prhs[] : array of pointers to input arguments
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	/* Macros for the ouput and input arguments */
	#define B_OUT plhs[0]
	#define A_IN  prhs[0]
	#define P_IN  prhs[1]
	double *B, *A, p, colnorm;
	int M, N, m, n;
	
	if((nrhs < 1) || (nrhs > 2))
	{
		mexErrMsgTxt("Wrong number of input arguments.");
	}
	else if(nlhs > 1)
	{
		mexErrMsgTxt("Too many output arguments.");
	}
	
	if(!IS_REAL_2D_FULL_DOUBLE(A_IN))
	{
		mexErrMsgTxt("A must be a real 2D full double array.");
	}
	
	/* If p is unspecified, set it to a default value. */
	if(nrhs == 1)
	{
		p = 2.0;
	}
	else 
	{
		if (!IS_REAL_2D_FULL_DOUBLE(P_IN))
			mexErrMsgTxt("P must be a real double scalar.");
		else
			p = mxGetScalar(P_IN);
	}
	
	/* Get the dimenisions */
	M = mxGetM(A_IN);
	N = mxGetN(A_IN);
	A = mxGetPr(A_IN);
	/* Create the output matrix */
	B_OUT = mxCreateDoubleMatrix(M, N, mxREAL);
	B = mxGetPr(B_OUT);
	
	for(n = 0; n < N; n++)
	{
		for(m = 0; m < M; m++)
			colnorm += pow(A[m + M * n], p);
		
		colnorm = pow(fabs(colnorm), 1.0/p);
		
		for(m = 0; m < M; m++)
			B[m + M * n] = A[m + M * n] / colnorm; 
	}
	
	return;
}