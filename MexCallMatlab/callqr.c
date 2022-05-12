/* call matlab's function */
#include <string.h>
#include "mex.h"

void DisplayMatrix(char *Name, double *Data, int M, int N)
{	/* Display matrix data */
	int m, n;
	mexPrintf("%s = \n", Name);
	for(m = 0; m < M; m++, mexPrintf("\n"))
		for(n = 0; n < N; n++)
			mexPrintf("%8.4f", Data[m + M * n]);
	
	return;
}

void CallQR(double *Data, int M, int N)
{	/* Perform QR factorization by calling the matlab function */
	mxArray *Q, *R, *A;
	mxArray *ppLhs[2];
	
	DisplayMatrix("Input", Data, M, N);
	A = mxCreateDoubleMatrix(M, N, mxREAL);
	memcpy(mxGetPr(A), Data, sizeof(double)*M*N);
	
	/* Call matlab's qr function */
	mexCallMATLAB(2, ppLhs, 1, &A, "qr");
	Q = ppLhs[0];
	R = ppLhs[1];
	DisplayMatrix("Q", mxGetPr(Q), M, N);
	DisplayMatrix("R", mxGetPr(R), M, N);
	
	mxDestroyArray(R);
	mxDestroyArray(Q);
	mxDestroyArray(A);
	
	return;
}

/* input*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	#define M_IN prhs[0]
	if(nrhs != 1 || mxGetNumberOfDimensions(M_IN) != 2 || !mxIsDouble(M_IN))
			mexErrMsgTxt("Invalid input.");
	
	CallQR(mxGetPr(M_IN), mxGetM(M_IN), mxGetN(M_IN));
	
	return;
}