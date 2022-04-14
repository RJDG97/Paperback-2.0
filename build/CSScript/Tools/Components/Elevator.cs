/**********************************************************************************
*\file         Elevator.cs
*\brief        Elevator.cs Script
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
    public unsafe class Elevator
    {
        private void* m_Address;
        UInt32 ID;

        public Elevator(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public float m_StartTime
        {
            get
            {
                return getstarttime(ID);
            }
            set
            {
                setstarttime(ID, value);
            }
        }

        public float m_StopTime
        {
            get
            {
                return getstoptime(ID);
            }
            set
            {
                setstoptime(ID, value);
            }
        }

        public bool m_UnitUnder
        {
            get
            {
                return getunitunder(ID);
            }

            set
            {
                setunitunder(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstarttime(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstarttime(UInt32 address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstoptime(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstoptime(UInt32 address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getunitunder(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setunitunder(UInt32 address, bool value);
    }
}
