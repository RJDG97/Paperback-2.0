using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class PlayerController
    {
        private void* m_Address;
        private UInt32 m_ID;

        public PlayerController(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_FPSMode
        {
            get
            {
                return GetFPSMode(m_Address);
            }
            set 
            {
                SetFPSMode(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFPSMode(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFPSMode(void* address, bool value);
    }
}
