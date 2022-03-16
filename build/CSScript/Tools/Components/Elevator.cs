using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Elevator
    {
        private void* m_Address;

        public Elevator(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public float m_StartTime
        {
            get
            {
                return getstarttime(m_Address);
            }
            set
            {
                setstarttime(m_Address, value);
            }
        }

        public float m_StopTime
        {
            get
            {
                return getstoptime(m_Address);
            }
            set
            {
                setstoptime(m_Address, value);
            }
        }

        public bool m_UnitUnder
        {
            get
            {
                return getunitunder(m_Address);
            }

            set
            {
                setunitunder(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstarttime(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstarttime(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstoptime(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstoptime(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getunitunder(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setunitunder(void* address, bool value);
    }
}
