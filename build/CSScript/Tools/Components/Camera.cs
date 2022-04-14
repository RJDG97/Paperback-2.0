/**********************************************************************************
*\file         Camera.cs
*\brief        Camera.cs Script
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
    public unsafe class Camera
    {
        UInt32 ID;
        public Camera(UInt32 id)
        {
            ID = id;
        }

        public bool m_Active
        {
            get
            {
                return getactive(ID);
            }
            set
            {
                setactive(ID, value);
            }
        }

        public Tools.MathLib.Vector3 m_Position
        {
            get
            {
                return getpos(ID);
            }
            set
            {
                setpos(ID, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactive(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactive(UInt32 address, bool mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getpos(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpos(UInt32 address, float x, float y, float z);
    }
}
