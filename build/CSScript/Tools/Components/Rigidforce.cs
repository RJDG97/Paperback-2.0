using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Rigidforce
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Rigidforce(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public float m_StaticFriction
        {
            get
            {
                return getstaticfriction(m_Address);
            }
            set
            {
                setstaticfriction(m_Address, value);
            }
        }

        public float m_DynamicFriction
        {
            get
            {
                return getdynamicfriction(m_Address);
            }
            set
            {
                setdynamicfriction(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_Forces
        {
            get
            {
                return getforces(m_Address);
            }
            set
            {
                setforces(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Momentum
        {
            get
            {
                return getmomentum(m_Address);
            }
            set
            {
                setmomentum(m_Address, value.x, value.y, value.z);
            }
        }

        public float m_Restitution
        {
            get
            {
                return getrestitution(m_Address);
            }
            set
            {
                setrestitution(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstaticfriction(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstaticfriction(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getdynamicfriction(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdynamicfriction(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getforces(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setforces(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmomentum(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmomentum(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getrestitution(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setrestitution(void* address, float value);
    }
}
