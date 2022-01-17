using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Socketed
    {
        private void* m_Address;
        private UInt32 m_ID;
        public Socketed(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_ParentSocket
        {
            get
            {
                return getparentsocket(m_Address);
            }
            set
            {
                //m_Offset = value;
                setparentsocket(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_SocketPosOffset
        {
            get
            {
                return getsocketposoffset(m_Address);
            }
            set
            {
                setsocketposoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_SocketRotOffset
        {
            get
            {
                return getsocketrotoffset(m_Address);
            }
            set
            {
                setsocketrotoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_SocketScaleOffset
        {
            get
            {
                return getsocketscaleoffset(m_Address);
            }
            set
            {
                setsocketscaleoffset(m_Address, value.x, value.y, value.z);
            }
        }

        public bool m_SyncAnimationWithParent
        {
            get
            {
                return getsyncanimationwithparent(m_Address);
            }
            set
            {
                setsyncanimationwithparent(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getparentsocket(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setparentsocket(void* address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getsocketposoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsocketposoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getsocketrotoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsocketrotoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getsocketscaleoffset(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsocketscaleoffset(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getsyncanimationwithparent(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsyncanimationwithparent(void* address, bool value);
    }
}
