#include "mex.h"

/* Count is a global variable, so it will be remembered between calls.*/
static int Count = 1;

void MyExit()
{
	mexPrintf("MyExit() called!\n");
	
	/* Do cleanup here ... */
	return;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	/* Register MyExit() to run when MEX-function is cleared */
	mexAtExit(&MyExit);
	mexPrintf("Count = %d\n", Count);
	Count++;
	return;
}