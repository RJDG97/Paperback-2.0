using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Rotation
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Rotation(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_Value
        {
            get
            {
                return getvalue(m_Address);
            }
            set
            {
                setvalue(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getvalue(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setvalue(void* address, float x, float y, float z);
    }
}
