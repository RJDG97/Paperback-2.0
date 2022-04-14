/**********************************************************************************
*\file         Sound.cs
*\brief        Sound.cs Script
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
    public unsafe class Sound
    {
        private void* m_Address;
        UInt32 m_ID;
        public Sound(UInt32 id)
        {
            m_ID = id;
            m_Address = getaddress(id);
        }

        public string m_SoundID
        {
            get
            {
                return getsoundid(m_ID);
            }
            set
            {
                setsoundid(m_ID, value);
            }
        }

        public UInt32 m_SoundPlayTag
        {
            get
            {
                return getsoundplaytag(m_ID);
            }
            set
            {
                setsoundplaytag(m_ID, value);
            }
        }

        public bool m_Is3DSound
        {
            get
            {
                return getis3dsound(m_ID);
            }
            set
            {
                setis3dsound(m_ID, value);
            }
        }

        public bool m_IsTriggerable
        {
            get
            {
                return getistriggerable(m_ID);
            }
            set
            {
                setistriggerable(m_ID, value);
            }
        }

        public bool m_Trigger
        {
            get
            {
                return gettrigger(m_ID);
            }
            set
            {
                settrigger(m_ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getsoundid(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsoundid(UInt32 ID, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getsoundplaytag(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setsoundplaytag(UInt32 ID, UInt32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getis3dsound(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setis3dsound(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getistriggerable(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setistriggerable(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool gettrigger(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settrigger(UInt32 ID, bool value);
    }
}
