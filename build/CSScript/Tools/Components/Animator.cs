/**********************************************************************************
*\file         Animatior.cs
*\brief        Animatior.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

        public Animator(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public string m_CurrentAnimationName
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

        public float m_PauseAtTime
        {
            get
            {
                return getpauseattime(m_Address);
            }
            set
            {
                setpauseattime(m_Address, value);
            }
        }

        public bool m_Reversed
        {
            get
            {
                return getreversed(m_Address);
            }
            set
            {
                setreversed(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getcurrentanimationname(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcurrentanimationname(void* address, string new_name);

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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getpauseattime(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpauseattime(void* address, float time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getreversed(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setreversed(void* address, bool reversed);
    }
}
