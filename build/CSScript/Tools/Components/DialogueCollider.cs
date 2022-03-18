using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class DialogueCollider
    {
        private void* m_Address;

        public DialogueCollider(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_DialogueName
        {
            get
            {
                return getdialoguename(m_Address);
            }
            set
            {
                setdialoguename(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getdialoguename(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdialoguename(void* address, String value);
    }
}
