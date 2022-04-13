/**********************************************************************************
*\file         Sphere.cs
*\brief        Sphere.cs Script
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
    public unsafe class Sphere
    {
        private void* m_Address;

        public Sphere(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public float m_Radius
        {
            get
            {
                return getradius(m_Address);
            }
            set
            {
                setradius(m_Address, value);
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
        private extern static float getradius(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setradius(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getcollided(void* address);
    }
}
