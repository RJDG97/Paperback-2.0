/**********************************************************************************
*\file         Offset.cs
*\brief        Offset.cs Script
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
    public unsafe class Offset
    {
        private void* m_Address;
        UInt32 ID;
        public Offset(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_PosOffset
        {
            get
            {
                return getposoffset(ID);
            }
            set
            {
                setposoffset(ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_RotOffset
        {
            get
            {
                return getrotoffset(ID);
            }
            set
            {
                setrotoffset(ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_ScaleOffset
        {
            get
            {
                return getscaleoffset(ID);
            }
            set
            {
                setscaleoffset(ID, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getposoffset(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setposoffset(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getrotoffset(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setrotoffset(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getscaleoffset(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setscaleoffset(UInt32 ID, float x, float y, float z);
    }
}
