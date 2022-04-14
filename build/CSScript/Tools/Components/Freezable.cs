﻿/**********************************************************************************
*\file         Freezable.cs
*\brief        Freezable.cs Script
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
    public unsafe class Freezable
    {
        private void* m_Address;
        UInt32 ID;

        public Freezable(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public bool m_Frozen
        {
            get
            {
                return getfrozen(ID);
            }
            set
            {
                setfrozen(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getfrozen(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setfrozen(UInt32 ID, bool value);
    }
}
