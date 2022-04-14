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
        UInt32 ID;
        public DialogueText(UInt32 id)
        {
            ID = id;
        }

        public String m_DialogueName
        {
            get
            {
                return getdialoguename(ID);
            }
            set
            {
                setdialoguename(ID, value);
            }
        }

        public float m_ElapsedTime
        {
            get
            {
                return getelapsedtime(ID);
            }
            set
            {
                setelapsedtime(ID, value);
            }
        }

        public UInt32 m_State
        {
            get
            {
                return getstate(ID);
            }
            set
            {
                setstate(ID, value);
            }
        }

        public Tools.MathLib.Vector3 m_InitialScale
        {
            get
            {
                return getinitialscale(ID);
            }
            set
            {
                setinitialscale(ID, value.x, value.y, value.z);
            }
        }

        public UInt32 m_Index
        {
            get
            {
                return getindex(ID);
            }
            set
            {
                setindex(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getdialoguename(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdialoguename(UInt32 address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getelapsedtime(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setelapsedtime(UInt32 address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getstate(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstate(UInt32 address, UInt32 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getinitialscale(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setinitialscale(UInt32 address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getindex(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setindex(UInt32 address, UInt32 value);
    }
}
