/**********************************************************************************
*\file         DialogueText.cs
*\brief        DialogueText.cs Script
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
    public unsafe class DialogueText
    {
        private void* m_Address;

        public DialogueText(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_DialogueName
        {
            get
            {
                return getdialoguename(m_Address);
            }
            set
            {
                setdialoguename(m_Address, value);
            }
        }

        public float m_ElapsedTime
        {
            get
            {
                return getelapsedtime(m_Address);
            }
            set
            {
                setelapsedtime(m_Address, value);
            }
        }

        public UInt32 m_State
        {
            get
            {
                return getstate(m_Address);
            }
            set
            {
                setstate(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_InitialScale
        {
            get
            {
                return getinitialscale(m_Address);
            }
            set
            {
                setinitialscale(m_Address, value.x, value.y, value.z);
            }
        }

        public UInt32 m_Index
        {
            get
            {
                return getindex(m_Address);
            }
            set
            {
                setindex(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getdialoguename(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdialoguename(void* address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getelapsedtime(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setelapsedtime(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getstate(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstate(void* address, UInt32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getinitialscale(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setinitialscale(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getindex(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setindex(void* address, UInt32 value);
    }
}
