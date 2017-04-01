#pragma once

#include "..\\Common\\Int128Array.h"

namespace ManagedSIMDArray
{

	public ref class Int128Array
	{
	public:
		Int128Array(int size32);
		Int128Array(int size32, int initialize);
		~Int128Array();

		property int default[int]
		{
		public:
			int get(int i)
			{
				return (*arr)(i);
			}

			void set(int i, int num)
			{
				(*arr)(i) = num;
			}
		}    

		property int Count
		{
		public:
			int get()
			{
				return arr->GetSize32();
			}    
		private:
			void set(int count)
			{
			}
		}

		property bool IsValid
		{
		public:
			bool get()
			{
				return arr != NULL;
			}    
		private:
			void set(bool valid)
			{
			}
		}

		void setUnusedValue(int num)
		{
			arr->setUnusedValue(num);
		}
		//private:
		// only C++/CLI can see public member, C# cannot see it.
		NativeSIMDArray::Int128Array* arr;
	};

} // NS ManagedSIMDArray