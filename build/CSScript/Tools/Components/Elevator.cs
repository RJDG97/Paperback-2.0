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
        private UInt32 m_ID;

        public Elevator(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Int32 m_StartFrame
        {
            get
            {
                return getstartframe(m_Address);
            }
            set
            {
                setstartframe(m_Address, value);
            }
        }

        public Int32 m_StopFrame
        {
            get
            {
                return getstopframe(m_Address);
            }
            set
            {
                setstopframe(m_Address, value);
            }
        }

        public bool m_Activated
        {
            get
            {
                return getactivated(m_Address);
            }

            set
            {
                setactivated(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Int32 getstartframe(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstartframe(void* address, Int32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Int32 getstopframe(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstopframe(void* address, Int32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactivated(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactivated(void* address, bool value);
    }
}
