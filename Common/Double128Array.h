#include <dvec.h>
#include <cassert>

namespace NativeSIMDArray
{

class Double128Array
{
public:
	//! Constructor
	Double128Array(int size64)
		:
		arr(NULL),
		size128(0),
		size64(0),
		remainder(0)
	{
		// divide by 4, to get the size in 128bit.
		size128 = (size64) >> 1;
		// calculate the remainder
		remainder = size64%2; 

#ifdef _DEBUG
		assert(remainder<2);
#endif // _DEBUG

		// if there is remainder, 
		// add 1 to the 128bit m_arraysize
		if(remainder!=0) 
			++size128;

		this->size64 = size64;

		size_t fullsize = (size128) * 16;

		arr = (F64vec2 *)(_aligned_malloc((size_t)(fullsize), 16));

		F64vec2 initialised(1.0, 1.0);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}

	//! Constructor
	Double128Array(int size64, double initialize)
		:
		arr(NULL),
		size128(0),
		size64(0),
		remainder(0)
	{
		// divide by 4, to get the size in 128bit.
		size128 = (size64) >> 1;
		// calculate the remainder
		remainder = size64%2; 

#ifdef _DEBUG
		assert(remainder<2);
#endif // _DEBUG

		// if there is remainder, 
		// add 1 to the 128bit m_arraysize
		if(remainder!=0) 
			++size128;

		this->size64 = size64;

		size_t fullsize = (size128) * 16;

		arr = (F64vec2 *)(_aligned_malloc((size_t)(fullsize), 16));

		F64vec2 initialised(initialize, initialize);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}

	//! Destructor
	~Double128Array()
	{
		if(arr!=NULL)
		{
			_aligned_free(arr);
			arr=NULL;
		}
	}

	//! overloaded operator to return 64 bit double
	double& operator()(size_t i)
	{
#ifdef _DEBUG
		assert(i<size64);
#endif // _DEBUG

		int a = i >> 1; // divide by 2
		int b = a << 1; // multiply by 2 again

		int rem = i - b; // compute the remainder

		return arr[a][rem];
	}

	//! overloaded operator to return 128 bit vector
	F64vec2& operator[](size_t i)
	{
#ifdef _DEBUG
		assert(i<size128);
#endif // _DEBUG

		return arr[i];
	}

	void setUnusedValue(double num)
	{
		if(size128==0||remainder==0)
			return;

		size_t unused = 2 - remainder;

		if(unused>=1)
			arr[size128-1][1] = num;
	}

	size_t GetSize128() { return size128; }
	size_t GetSize64() { return size64; }
	size_t GetRemainder() { return remainder; }

private:
	//! point to array of 128 bit elements
	F64vec2* arr;
	//! size of array in 128 bit chunks
	size_t size128;
	//! size of array in 64 bit chunks (might be (64 * x) < (128/2 * x)
	size_t size64;
	//! remainder of 64 bit element at the last 128 bit element.
	size_t remainder;
};

} // NS NativeSIMDArray