using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Rigidbody
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Rigidbody(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_Accel
        {
            get
            {
                return getaccel(m_Address);
            }
            set
            {
                setaccel(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Velocity
        {
            get
            {
                return getvelocity(m_Address);
            }
            set
            {
                setvelocity(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getaccel(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setaccel(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getvelocity(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setvelocity(void* address, float x, float y, float z);
    }
}
