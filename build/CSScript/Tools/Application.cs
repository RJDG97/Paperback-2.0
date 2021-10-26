using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    class Application
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Quit();
    }
}
