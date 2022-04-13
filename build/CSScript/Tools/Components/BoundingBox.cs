/**********************************************************************************
*\file         BoundingBox.cs
*\brief        BoundingBox.cs Script
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
    public unsafe class BoundingBox
    {
        private void* m_Address;

        public BoundingBox(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 Min
        {
            get
            {
                return getmin(m_Address);
            }
            set
            {
                setmin(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 Max
        {
            get
            {
                return getmax(m_Address);
            }
            set
            {
                setmax(m_Address, value.x, value.y, value.z);
            }
        }

        public bool m_Collided
        {
            get
            {
                return getcollided(m_Address);
            }
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmin(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmin(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmax(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmax(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getcollided(void* address);
    }
}
