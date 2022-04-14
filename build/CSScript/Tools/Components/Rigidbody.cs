/**********************************************************************************
*\file         Rigidbody.cs
*\brief        Rigidbody.cs Script
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
    public unsafe class Rigidbody
    {
        private void* m_Address;

        public Rigidbody(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public Tools.MathLib.Vector3 m_Accel
        {
            get
            {
                return getaccel(m_Address);
            }
            set
            {
                setaccel(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Velocity
        {
            get
            {
                return getvelocity(m_Address);
            }
            set
            {
                setvelocity(m_Address, value.x, value.y, value.z);
            }
        }

        public float VelocityMagSq()
        {

            return VelocityMagnitudeSquared(m_Address);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getaccel(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setaccel(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getvelocity(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setvelocity(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float VelocityMagnitudeSquared(void* address);
    }
}
