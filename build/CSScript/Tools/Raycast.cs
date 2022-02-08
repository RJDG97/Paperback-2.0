using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript.Tools
{
    public class Raycast
    {
        public UInt32[] RayAABB(UInt32 ID)
        {
            return rayaab(ID);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static UInt32[] rayaab(UInt32 id);
    }
}
