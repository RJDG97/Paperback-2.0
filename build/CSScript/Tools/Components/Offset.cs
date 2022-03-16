using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Offset
    {
        private void* m_Address;

        public Offset(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_PosOffset
        {
            get
            {
                return getposoffset(m_Address);
            }
            set
            {
                setposoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_RotOffset
        {
            get
            {
                return getrotoffset(m_Address);
            }
            set
            {
                setrotoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_ScaleOffset
        {
            get
            {
                return getscaleoffset(m_Address);
            }
            set
            {
                setscaleoffset(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getposoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setposoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getrotoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setrotoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getscaleoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setscaleoffset(void* address, float x, float y, float z);
    }
}
