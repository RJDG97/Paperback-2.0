using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{

    public class Test
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void print(string str);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static string internaltest();

        public void externaltest()
        {
            print(internaltest());
            print("Mono External test");
        }
    }
    public class Main
    {
        public static void main()
        {
            Test.print("C# Main");
        }

        // Need to return an instance of the class for external calls
        public static Test testinst()
        { return new Test(); }
    }
}
