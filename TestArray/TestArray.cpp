// TestArray.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\\Common\\Int128Array.h"
//#include "UInt128Array.h"
#include "..\\Common\\Float128Array.h"
#include "..\\Common\\Double128Array.h"

//// test unsigned integer
//int _tmain(int argc, _TCHAR* argv[])
//{
//	using namespace std;
//
//	UInt128Array uintArr(5, 20);
//	uintArr.setUnusedValue(6);
//
//	Iu32vec4 div(3,3,3,3);
//
//	for(size_t i=0; i<uintArr.GetSize128(); ++i)
//	{
//		uintArr[i] = uintArr[i] % div;
//	}
//	for(size_t i=0; i<uintArr.GetSize128(); ++i)
//	{
//		for(size_t j=0; j<4; ++j)
//		cout << uintArr[i][j] <<endl;
//	}
//	cout<<"Remainder:"<<uintArr.GetRemainder()<<endl;
//	for(size_t k=0; k<uintArr.GetSize32(); ++k)
//	{
//		cout << uintArr(k) <<endl;
//	}
//
//	return 0;
//}

//// test integer
//int _tmain(int argc, _TCHAR* argv[])
//{
//	using namespace std;
//
//	using namespace NativeSIMDArray;
//
//	Int128Array intArr(5, 20);
//	intArr.setUnusedValue(6);
//
//	Is32vec4 div(3,3,3,3);
//
//	for(size_t i=0; i<intArr.GetSize128(); ++i)
//	{
//		intArr[i] = intArr[i] % div;
//	}
//	for(size_t i=0; i<intArr.GetSize128(); ++i)
//	{
//		for(size_t j=0; j<4; ++j)
//			cout << intArr[i][j] <<endl;
//	}
//	cout<<"Remainder:"<<intArr.GetRemainder()<<endl;
//	for(size_t k=0; k<intArr.GetSize32(); ++k)
//	{
//		intArr(k) = 1;
//		cout << intArr(k) <<endl;
//	}
//
//	return 0;
//}

// test float
int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	using namespace NativeSIMDArray;

	Float128Array floatArr(7, 2.0f);
	floatArr.setUnusedValue(12.5f);

	floatArr[0] = floatArr[0] + floatArr[0];
	for(size_t i=0; i<floatArr.GetSize128(); ++i)
	{
		for(size_t j=0; j<4; ++j)
			cout << floatArr[i][j] <<endl;
	}
	cout<<"Remainder:"<<floatArr.GetRemainder()<<endl;
	for(size_t k=0; k<floatArr.GetSize32(); ++k)
	{
		cout << floatArr(k) <<endl;
	}

	return 0;
}

//// test double
//int _tmain(int argc, _TCHAR* argv[])
//{
//	using namespace std;
//
//	Double128Array doubleArr(6, 2.0);
//	doubleArr.setUnusedValue(12.5);
//
//	for(size_t i=0; i<doubleArr.GetSize128(); ++i)
//	{
//		for(size_t j=0; j<2; ++j)
//			cout << doubleArr[i][j] <<endl;
//	}
//	cout<<"Remainder:"<<doubleArr.GetRemainder()<<endl;
//	for(size_t k=0; k<doubleArr.GetSize64(); ++k)
//	{
//		cout << doubleArr(k) <<endl;
//	}
//
//	return 0;
//}
