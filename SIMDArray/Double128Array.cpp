#include "StdAfx.h"
#include "Double128Array.h"


using namespace ManagedSIMDArray;

Double128Array::Double128Array(int size32)
{
	arr = NULL;
	arr = new NativeSIMDArray::Double128Array(size32);
}

Double128Array::Double128Array(int size32, double initialize)
{
	arr = NULL;
	arr = new NativeSIMDArray::Double128Array(size32, initialize);
}

Double128Array::~Double128Array()
{
	if(arr!=NULL)
	{
		delete arr;
		arr = NULL;
	}
}

