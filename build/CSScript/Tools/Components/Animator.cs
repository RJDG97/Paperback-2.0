using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Animator
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Animator(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_CurrentAnimationName
        {
            get
            {
                return getcurrentanimationname(m_Address);
            }
            set
            {
                setcurrentanimationname(m_Address, value);
            }
        }

        public float m_CurrentTime
        {
            get
            {
                return getcurrentanimationtime(m_Address);
            }
            set
            {
                setcurrentanimationtime(m_Address, value);
            }
        }

        public bool m_PlayOnce
        {
            get
            {
                return getplayonce(m_Address);
            }
            set
            {
                setplayonce(m_Address, value);
            }
        }

        public bool m_FinishedAnimating
        {
            get
            {
                return getfinishedanimating(m_Address);
            }
        }

        public bool m_PauseAnimation
        {
            get
            {
                return getpauseanimation(m_Address);
            }
            set
            {
                setpauseanimation(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getcurrentanimationname(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcurrentanimationname(void* address, String new_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getcurrentanimationtime(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcurrentanimationtime(void* address, float time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getplayonce(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setplayonce(void* address, bool play_once);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getfinishedanimating(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getpauseanimation(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpauseanimation(void* address, bool pause_animation);
    }
}
