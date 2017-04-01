#pragma once
#include "..\\Common\\Double128Array.h"

namespace ManagedSIMDArray
{
	public ref class Double128Array
	{
	public:
		Double128Array(int size32);
		Double128Array(int size32, double initialize);
		~Double128Array();

		property double default[int]
		{
		public:
			double get(int i)
			{
				return (*arr)(i);
			}

			void set(int i, double num)
			{
				(*arr)(i) = num;
			}
		}    

		property int Count
		{
		public:
			int get()
			{
				return arr->GetSize64();
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
		void setUnusedValue(double num)
		{
			arr->setUnusedValue(num);
		}

		//private:
		// only C++/CLI can see public member, C# cannot see it.
		NativeSIMDArray::Double128Array* arr;
	};

} // NS ManagedSIMDArray