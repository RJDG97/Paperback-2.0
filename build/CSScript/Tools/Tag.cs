using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript.Tools
{
    public class Tag
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsPushable(UInt32 ID);
    }
}
