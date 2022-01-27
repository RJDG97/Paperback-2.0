using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Mesh
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Mesh(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public string m_Model
        {
            get
            {
                return getmodel(m_Address);
            }
            set
            {
                setmodel(m_Address, value);
            }
        }

        public string m_Texture
        {
            get
            {
                return gettexture(m_Address);
            }
            set
            {
                settexture(m_Address, value);
            }
        }

        public int m_Bias
        {
            get
            {
                return getbias(m_Address);
            }
            set
            {
                setbias(m_Address, value);
            }
        }

        public bool m_Active
        {
            get
            {
                return getactive(m_Address);
            }
            set
            {
                setactive(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getmodel(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmodel(void* address, string model);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string gettexture(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settexture(void* address, string texture);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int getbias(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setbias(void* address, int bias);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactive(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactive(void* address, bool active);
    }
}
