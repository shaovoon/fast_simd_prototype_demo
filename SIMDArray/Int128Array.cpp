#include "StdAfx.h"
#include "Int128Array.h"

using namespace ManagedSIMDArray;

Int128Array::Int128Array(int size32)
{
	arr = NULL;
	arr = new NativeSIMDArray::Int128Array(size32);
}

Int128Array::Int128Array(int size32, int initialize)
{
	arr = NULL;
	arr = new NativeSIMDArray::Int128Array(size32, initialize);
}

Int128Array::~Int128Array()
{
	if(arr!=NULL)
	{
		delete arr;
		arr = NULL;
	}
}
