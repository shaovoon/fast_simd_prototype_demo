#pragma once
#include "Double128Array.h"
#include "Float128Array.h"
#include "Int128Array.h"
#include "NativeTestCompute.h"

namespace ManagedSIMDArray
{
public ref class TestCompute
{
public:
	TestCompute(void);

	static void ComputeInt(ManagedSIMDArray::Int128Array^ a, ManagedSIMDArray::Int128Array^ b)
	{
		NativeTestCompute::ComputeInt(a->arr, b->arr);
	}
	static void ComputeFloat(ManagedSIMDArray::Float128Array^ a, ManagedSIMDArray::Float128Array^ b)
	{
		NativeTestCompute::ComputeFloat(a->arr, b->arr);
	}
	static void ComputeDouble(ManagedSIMDArray::Double128Array^ a, ManagedSIMDArray::Double128Array^ b)
	{
		NativeTestCompute::ComputeDouble(a->arr, b->arr);
	}

};

} // NS ManagedSIMDArray