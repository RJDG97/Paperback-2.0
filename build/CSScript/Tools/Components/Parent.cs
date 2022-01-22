using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Parent
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Parent(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public UInt32 m_ChildID
        {
            get
            {
                return getchildid(m_Address);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getchildid(void* address);
    }
}
