#pragma once
#include "Double128Array.h"
#include "Float128Array.h"
#include "Int128Array.h"

#pragma unmanaged
class NativeTestCompute
{
public:
	NativeTestCompute(void);
	~NativeTestCompute(void);

	static void ComputeInt(NativeSIMDArray::Int128Array* a, NativeSIMDArray::Int128Array* b)
	{
		for(size_t i=0; i<a->GetSize128(); ++i)
		{
			(*a)[i] = (*a)[i] * (*b)[i];
		}
	}
	static void ComputeFloat(NativeSIMDArray::Float128Array* a, NativeSIMDArray::Float128Array* b)
	{
		for(size_t i=0; i<a->GetSize128(); ++i)
		{
			(*a)[i] = (*a)[i] * (*b)[i];
		}
	}
	static void ComputeDouble(NativeSIMDArray::Double128Array* a, NativeSIMDArray::Double128Array* b)
	{
		for(size_t i=0; i<a->GetSize128(); ++i)
		{
			(*a)[i] = (*a)[i] * (*b)[i];
		}
	}

};

#pragma managed
