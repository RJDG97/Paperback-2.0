using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Name
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Name(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_Name
        {
            get
            {
                return getname(m_Address);
            }
            set
            {
                setname(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getname(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setname(void* address, String value);
    }
}
