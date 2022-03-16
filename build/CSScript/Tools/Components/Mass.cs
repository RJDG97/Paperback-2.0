using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Mass
    {
        private void* m_Address;

        public Mass(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public float m_Mass
        {
            get
            {
                return getmass(m_Address);
            }
            set
            {
                setmass(m_Address, value);
            }
        }

        public float m_InvMass
        {
            get
            {
                return getinvmass(m_Address);
            }
            set
            {
                setinvmass(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getmass(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmass(void* address, float mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getinvmass(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setinvmass(void* address, float inv_mass);
    }
}
