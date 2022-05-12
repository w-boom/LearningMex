/* calculte function evaluations, and remember function evaluations 
 * have already computed;
 */
#include "mex.h"

#define INITIAL_TABLE_CAPACITY 64

/* Table for holding precomputed values */
static struct{
	double *X;
	double *Y;
	int Size;
	int Capacity;
}Table = {0, 0, 0, 0};

void ReallocTable(int NewCapacity)
{   /* (Re)allocate the table */
	if(!(Table.X = (double *) mxRealloc(Table.X, sizeof(double)*NewCapacity))
		|| !(Table.Y = (double *)mxRealloc(Table.Y, sizeof(double)*NewCapacity)))
	{
		mexErrMsgTxt("Out of memory");
	}
	mexMakeMemoryPersistent(Table.X);
	mexMakeMemoryPersistent(Table.Y);
	Table.Capacity = NewCapacity;
	
	return;
}

void AddToTable(double x, double y)
{	/* Add(x,y) to the table */
	if(Table.Size == Table.Capacity)
		ReallocTable(2 * Table.Capacity);
	Table.X[Table.Size] = x;
	Table.Y[Table.Size++] = y;
	
	return;
}

mxArray *EvaluateFunction(const mxArray *pFunction, const mxArray *px)
{	/* Evalute function handle pFunction at px */
	const mxArray *ppFevalRhs[2] = {pFunction, px};
	mxArray *py;
	mexPrintf("Evaluating f(x = %g)\n", mxGetScalar(px));
	mexCallMATLAB(1, &py, 2, (mxArray **)ppFevalRhs, "feval");
	
	return py;
}

void MyExit()
{
	mxFree(Table.X);
	mxFree(Table.Y);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	#define FCN_IN prhs[0]
	#define X_IN   prhs[1]
	#define Y_OUT  plhs[0]
	double x, *y;
	int i;
	
	if(nrhs != 2)
	{
		mexErrMsgTxt("Two input arguments required.");
	}
	else if(mxGetClassID(FCN_IN) != mxFUNCTION_CLASS && mxGetClassID(FCN_IN) != mxCHAR_CLASS)
	{
		mexErrMsgTxt("First argument should be a function handle.");
	}
	else if(!mxIsDouble(X_IN) || mxGetNumberOfElements(X_IN) != 1)
	{
		mexErrMsgTxt("X must be a real double scalar.");
	}
	
	x = mxGetScalar(X_IN);
	mexAtExit(&MyExit);
	
	if(!Table.X || !Table.Y)
		ReallocTable(INITIAL_TABLE_CAPACITY);
	
	/* Search for x in the table;*/
	for(i = 0; i < Table.Size; i++)
	{
		if(x == Table.X[i])
		{
			mexPrintf("Found precomputed value for x = %g\n", x);
			y = mxGetPr(Y_OUT = mxCreateDoubleMatrix(1, 1, mxREAL));
			*y = Table.Y[i];
			return;
		}
	}
	
	/* x is not yet in the table */
	Y_OUT = EvaluateFunction(FCN_IN, X_IN);
	AddToTable(x, mxGetScalar(Y_OUT));
	return;
}
