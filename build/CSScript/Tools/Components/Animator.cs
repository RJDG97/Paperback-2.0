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
        UInt32 ID;
        public Animator(UInt32 id)
        {
            ID = id;
        }

        public string m_CurrentAnimationName
        {
            get
            {
                return getcurrentanimationname(ID);
            }
            set
            {
                setcurrentanimationname(ID, value);
            }
        }

        public float m_CurrentTime
        {
            get
            {
                return getcurrentanimationtime(ID);
            }
            set
            {
                setcurrentanimationtime(ID, value);
            }
        }

        public bool m_PlayOnce
        {
            get
            {
                return getplayonce(ID);
            }
            set
            {
                setplayonce(ID, value);
            }
        }

        public bool m_FinishedAnimating
        {
            get
            {
                return getfinishedanimating(ID);
            }
        }

        public bool m_PauseAnimation
        {
            get
            {
                return getpauseanimation(ID);
            }
            set
            {
                setpauseanimation(ID, value);
            }
        }

        public float m_PauseAtTime
        {
            get
            {
                return getpauseattime(ID);
            }
            set
            {
                setpauseattime(ID, value);
            }
        }

        public bool m_Reversed
        {
            get
            {
                return getreversed(ID);
            }
            set
            {
                setreversed(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getcurrentanimationname(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcurrentanimationname(UInt32 address, string new_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getcurrentanimationtime(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcurrentanimationtime(UInt32 address, float time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getplayonce(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setplayonce(UInt32 address, bool play_once);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getfinishedanimating(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getpauseanimation(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpauseanimation(UInt32 address, bool pause_animation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getpauseattime(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpauseattime(UInt32 address, float time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getreversed(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setreversed(UInt32 address, bool reversed);
    }
}
