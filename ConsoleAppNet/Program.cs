using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ManagedSIMDArray;

namespace ConsoleAppNet
{
    class Program
    {
        static void Main(string[] args)
        {
            TestInt();
            TestFloat();
            TestDouble();
        }

        static void TestInt()
        {
            Console.WriteLine("Testing integers");
            ManagedSIMDArray.Int128Array a = new ManagedSIMDArray.Int128Array(7, 2);
            ManagedSIMDArray.Int128Array b = new ManagedSIMDArray.Int128Array(7, 5);

            ManagedSIMDArray.TestCompute.ComputeInt(a, b);

            for (int i = 0; i < a.Count; ++i)
                Console.WriteLine("{0}", a[i]);
        }
        static void TestFloat()
        {
            Console.WriteLine("Testing floats");
            ManagedSIMDArray.Float128Array a = new ManagedSIMDArray.Float128Array(7, 2.0f);
            ManagedSIMDArray.Float128Array b = new ManagedSIMDArray.Float128Array(7, 8.0f);

            ManagedSIMDArray.TestCompute.ComputeFloat(a, b);

            for (int i = 0; i < a.Count; ++i)
                Console.WriteLine("{0}", a[i]);
        }
        static void TestDouble()
        {
            Console.WriteLine("Testing doubles");
            ManagedSIMDArray.Double128Array a = new ManagedSIMDArray.Double128Array(7, 2.0);
            ManagedSIMDArray.Double128Array b = new ManagedSIMDArray.Double128Array(7, 15.0);

            ManagedSIMDArray.TestCompute.ComputeDouble(a, b);

            for (int i = 0; i < a.Count; ++i)
                Console.WriteLine("{0}", a[i]);
        }
    }
}
