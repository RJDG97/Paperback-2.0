using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class AABB
    {
        private void* m_Address;
        private UInt32 m_ID;

        public AABB(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public int m_Collided
        {
            get
            {
                return getcollided(m_Address);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int getcollided(void* address);
    }
}
