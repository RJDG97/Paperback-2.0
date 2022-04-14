/**********************************************************************************
*\file         Transform.cs
*\brief        Transform.cs Script
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
    public unsafe class Transform
    {
        UInt32 ID;
        public Transform(UInt32 id)
        {
            ID = id;
        }

        public Tools.MathLib.Vector3 m_Offset
        {
            get
            {
                return getoffset(ID);
            }
            set
            {
                setoffset(ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Position
        {
            get
            {
                return getposition(ID);
            }
            set
            {
                //m_Position = value;
                setposition(ID, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getoffset(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setoffset(UInt32 address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getposition(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setposition(UInt32 address, float x, float y, float z);
    }
}
