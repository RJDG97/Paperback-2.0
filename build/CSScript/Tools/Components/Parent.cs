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

        public Parent(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Int32 GetChildIDofName(String child_name)
        {
            return getchildidofname(m_Address, child_name);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Int32 getchildidofname(void* address, string name);
    }
}
