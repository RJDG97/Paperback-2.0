using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Text
    {
        private void* m_Address;

        public Text(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_Font
        {
            get
            {
                return getfont(m_Address);
            }
            set
            {
                setfont(m_Address, value);
            }
        }

        public String m_Text
        {
            get
            {
                return gettext(m_Address);
            }
            set
            {
                settext(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_Color
        {
            get
            {
                return getcolor(m_Address);
            }
            set
            {
                setcolor(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getfont(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setfont(void* address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String gettext(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settext(void* address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getcolor(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcolor(void* address, float x, float y, float z);
    }
}
