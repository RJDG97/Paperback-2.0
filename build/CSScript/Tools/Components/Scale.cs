/**********************************************************************************
*\file         Scale.cs
*\brief        Scale.cs Script
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
    public unsafe class Scale
    {
        UInt32 m_ID;
        public Scale(UInt32 id)
        {
            m_ID = id;
        }

        public Tools.MathLib.Vector3 m_Value
        {
            get
            {
                return getvalue(m_ID);
            }
            set
            {
                setvalue(m_ID, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getvalue(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setvalue(UInt32 ID, float x, float y, float z);
    }
}
