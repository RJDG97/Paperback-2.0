using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Child
    {
        private void* m_Address;
        private UInt32 m_ID;
        public Transform(UInt32 id)
        {
            // m_ID = ID;
            id = 40; // For testing (Cardback)
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_Offset
        {
            get
            {
                return getoffset(m_Address);
            }
            set
            {
                //m_Offset = value;
                setoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Position
        {
            get
            {
                return getposition(m_Address);
            }
            set
            {
                //m_Position = value;
                setposition(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getposition(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setposition(void* address, float x, float y, float z);
    }
}
