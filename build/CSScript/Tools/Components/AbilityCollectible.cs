using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class AbilityCollectible
    {
        private void* m_Address;
        private UInt32 m_ID;

        public AbilityCollectible(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_Grow
        {
            get
            {
                return GetGrowStatus( m_Address );
            }
            set
            {
                SetGrowStatus( m_Address, value );
            }
        }

        public bool m_Shrink
        {
            get
            {
                return GetShrinkStatus( m_Address );
            }
            set
            {
                SetShrinkStatus( m_Address, value );
            }
        }

        public bool m_Freeze
        {
            get
            {
                return GetFreezeStatus( m_Address );
            }
            set
            {
                SetFreezeStatus( m_Address, value );
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetGrowStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetShrinkStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFreezeStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetGrowStatus(void* address, bool status);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetShrinkStatus(void* address, bool status);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFreezeStatus(void* address, bool status);
    }
}
