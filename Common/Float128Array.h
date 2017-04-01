#include <fvec.h>
#include <cassert>

namespace NativeSIMDArray
{

class Float128Array
{
public:
	//! Constructor
	Float128Array(int size32)
		:
		arr(NULL),
		size128(0),
		size32(0),
		remainder(0)
	{
		// divide by 4, to get the size in 128bit.
		size128 = (size32) >> 2;
		// calculate the remainder
		remainder = size32%4; 

#ifdef _DEBUG
		assert(remainder<4);
#endif // _DEBUG

		// if there is remainder, 
		// add 1 to the 128bit m_arraysize
		if(remainder!=0) 
			++size128;

		this->size32 = size32;

		size_t fullsize = (size128) * 16;

		arr = (F32vec4 *)(_aligned_malloc((size_t)(fullsize), 16));

		F32vec4 initialised(1.0f,1.0f,1.0f,1.0f);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}
	//! Constructor
	Float128Array(int size32, float initialise)
		:
		arr(NULL),
		size128(0),
		size32(0),
		remainder(0)
	{
		// divide by 4, to get the size in 128bit.
		size128 = (size32) >> 2;
		// calculate the remainder
		remainder = size32%4; 

#ifdef _DEBUG
		assert(remainder<4);
#endif // _DEBUG

		// if there is remainder, 
		// add 1 to the 128bit m_arraysize
		if(remainder!=0) 
			++size128;

		this->size32 = size32;

		size_t fullsize = (size128) * 16;

		arr = (F32vec4 *)(_aligned_malloc((size_t)(fullsize), 16));

		F32vec4 initialised(initialise,initialise,initialise,initialise);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}
	//! Destructor
	~Float128Array()
	{
		if(arr!=NULL)
		{
			_aligned_free(arr);
			arr=NULL;
		}
	}

	//! overloaded operator to return 32 bit float
	float& operator()(size_t i)
	{
#ifdef _DEBUG
		assert(i<size32);
#endif // _DEBUG

		int a = i >> 2; // divide by 4
		int b = a << 2; // multiply by 4 again

		int rem = i - b; // compute the remainder

		return arr[a][rem];
	}

	//! overloaded operator to return 128 bit vector
	F32vec4& operator[](size_t i)
	{
#ifdef _DEBUG
		assert(i<size128);
#endif // _DEBUG

		return arr[i];
	}

	void setUnusedValue(float num)
	{
		if(size128==0||remainder==0)
			return;

		size_t unused = 4 - remainder;

		if(unused>=1)
			arr[size128-1][3] = num;
		if(unused>=2)
			arr[size128-1][2] = num;
		if(unused>=3)
			arr[size128-1][1] = num;
	}

	size_t GetSize128() { return size128; }
	size_t GetSize32() { return size32; }
	size_t GetRemainder() { return remainder; }

private:
	//! point to array of 128 bit elements
	F32vec4* arr;
	//! size of array in 128 bit chunks
	size_t size128;
	//! size of array in 32 bit chunks (might be (32 * x) < (128/4 * x)
	size_t size32;
	//! Remainder of 32 bit element at the last 128 bit element.
	size_t remainder;
};

} // NS NativeSIMDArray