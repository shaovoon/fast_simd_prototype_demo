#include "StdAfx.h"
#include "Float128Array.h"


using namespace ManagedSIMDArray;

Float128Array::Float128Array(int size32)
{
	arr = NULL;
	arr = new NativeSIMDArray::Float128Array(size32);
}

Float128Array::Float128Array(int size32, float initialize)
{
	arr = NULL;
	arr = new NativeSIMDArray::Float128Array(size32, initialize);
}

Float128Array::~Float128Array()
{
	if(arr!=NULL)
	{
		delete arr;
		arr = NULL;
	}
}

