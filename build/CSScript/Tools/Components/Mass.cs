/**********************************************************************************
*\file         Mass.cs
*\brief        Mass.cs Script
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
    public unsafe class Mass
    {
        UInt32 ID;

        public Mass(UInt32 id)
        {
            ID = id;
        }

        public float m_Mass
        {
            get
            {
                return getmass(ID);
            }
            set
            {
                setmass(ID, value);
            }
        }

        public float m_InvMass
        {
            get
            {
                return getinvmass(ID);
            }
            set
            {
                setinvmass(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getmass(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmass(UInt32 ID, float mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getinvmass(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setinvmass(UInt32 ID, float inv_mass);
    }
}
