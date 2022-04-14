/**********************************************************************************
*\file         Text.cs
*\brief        Text.cs Script
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
    public unsafe class Text
    {
        UInt32 ID;

        public Text(UInt32 id)
        {
            ID = id;
        }

        public String m_Font
        {
            get
            {
                return getfont(ID);
            }
            set
            {
                setfont(ID, value);
            }
        }

        public String m_Text
        {
            get
            {
                return gettext(ID);
            }
            set
            {
                settext(ID, value);
            }
        }

        public Tools.MathLib.Vector3 m_Color
        {
            get
            {
                return getcolor(ID);
            }
            set
            {
                setcolor(ID, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getfont(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setfont(UInt32 address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String gettext(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settext(UInt32 address, String value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getcolor(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcolor(UInt32 address, float x, float y, float z);
    }
}
