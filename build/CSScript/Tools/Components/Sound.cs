using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Sound
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Sound(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public string m_SoundID
        {
            get
            {
                return getsoundid(m_Address);
            }
            set
            {
                setsoundid(m_Address, value);
            }
        }

        public UInt32 m_SoundPlayTag
        {
            get
            {
                return getsoundplaytag(m_Address);
            }
            set
            {
                setsoundplaytag(m_Address, value);
            }
        }

        public bool m_Is3DSound
        {
            get
            {
                return getis3dsound(m_Address);
            }
            set
            {
                setis3dsound(m_Address, value);
            }
        }

        public bool m_IsTriggerable
        {
            get
            {
                return getistriggerable(m_Address);
            }
            set
            {
                setistriggerable(m_Address, value);
            }
        }

        public bool m_Trigger
        {
            get
            {
                return gettrigger(m_Address);
            }
            set
            {
                settrigger(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getsoundid(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsoundid(void* address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getsoundplaytag(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsoundplaytag(void* address, UInt32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getis3dsound(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setis3dsound(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getistriggerable(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setistriggerable(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool gettrigger(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settrigger(void* address, bool value);
    }
}
