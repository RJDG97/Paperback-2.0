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
        private void* m_Address;

        public Camera(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_Active
        {
            get
            {
                return getactive(m_Address);
            }
            set
            {
                setactive(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_Position
        {
            get
            {
                return getpos(m_Address);
            }
            set
            {
                setpos(m_Address, value.x, value.y, value.z);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactive(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactive(void* address, bool mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getpos(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpos(void* address, float x, float y, float z);
    }
}
