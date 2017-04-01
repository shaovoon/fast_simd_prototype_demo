#pragma once
#include "..\\Common\\Float128Array.h"

namespace ManagedSIMDArray
{
	public ref class Float128Array
	{
	public:
		Float128Array(int size32);
		Float128Array(int size32, float initialize);
		~Float128Array();

		property float default[int]
		{
		public:
			float get(int i)
			{
				return (*arr)(i);
			}

			void set(int i, float num)
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
		void setUnusedValue(float num)
		{
			arr->setUnusedValue(num);
		}

	//private:
		// only C++/CLI can see public member, C# cannot see it.
		NativeSIMDArray::Float128Array* arr;
	};

} // NS ManagedSIMDArray