#include <dvec.h>
#include <cassert>

class UInt128Array
{
public:
	//! Constructor
	UInt128Array(int size32)
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

		arr = (Iu32vec4 *)(_aligned_malloc((size_t)(fullsize), 16));

		Iu32vec4 initialised(1,1,1,1);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}

	//! Constructor
	UInt128Array(int size32, unsigned int initialize)
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

		arr = (Iu32vec4 *)(_aligned_malloc((size_t)(fullsize), 16));

		Iu32vec4 initialised(initialize,initialize,initialize,initialize);

		for(size_t i=0; i<size128; ++i)
		{
			arr[i] = initialised;
		}
	}

	//! Destructor
	~UInt128Array()
	{
		if(arr!=NULL)
		{
			_aligned_free(arr);
			arr=NULL;
		}
	}

	//! overloaded operator to return 32 bit integer
	unsigned int operator()(size_t i)
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
	Iu32vec4& operator[](size_t i)
	{
#ifdef _DEBUG
		assert(i<size128);
#endif // _DEBUG

		return arr[i];
	}

	void setUnused(int num)
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
	Iu32vec4* arr;
	//! size of array in 128 bit chunks
	size_t size128;
	//! size of array in 32 bit chunks (might be (32 * x) < (128/4 * x)
	size_t size32;
	//! Remainder of 32 bit element at the last 128 bit element.
	size_t remainder;
};

static inline Iu32vec4 operator * (Iu32vec4 const & a,
	Iu32vec4 const & b) {
		__m128i a13, b13, prod02, prod13, prod01, prod23, prod0123;
		a13 = _mm_shuffle_epi32(a, 0xF5); // (-,a3,-,a1)
		b13 = _mm_shuffle_epi32(b, 0xF5); // (-,b3,-,b1)
		prod02 = _mm_mul_epu32(a, b); // (-,a2*b2,-,a0*b0)
		prod13 = _mm_mul_epu32(a13, b13); // (-,a3*b3,-,a1*b1)
		prod01 = _mm_unpacklo_epi32(prod02,prod13); // (-,-,a1*b1,a0*b0)
		prod23 = _mm_unpackhi_epi32(prod02,prod13); // (-,-,a3*b3,a2*b2)
		prod0123 = _mm_unpacklo_epi64(prod01,prod23); // (ab3,ab2,ab1,ab0)
		return prod0123;
}

static inline Iu32vec4 operator / (Iu32vec4 const & a,
	Iu32vec4 const & b) {
		Iu32vec4 ans;
		ans[0] = a[0] / b[0];
		ans[1] = a[1] / b[1];
		ans[2] = a[2] / b[2];
		ans[3] = a[3] / b[3];
		return ans;
}

static inline Iu32vec4 operator % (Iu32vec4 const & a,
	Iu32vec4 const & b) {
		Iu32vec4 ans;
		ans[0] = a[0] % b[0];
		ans[1] = a[1] % b[1];
		ans[2] = a[2] % b[2];
		ans[3] = a[3] % b[3];
		return ans;
}