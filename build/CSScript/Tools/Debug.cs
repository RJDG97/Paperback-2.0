using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Debug
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Log(string str);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern static void DrawLine(vector3 begin ,vector3 end, Color colour);
    }
}
