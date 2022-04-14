/**********************************************************************************
*\file         AbilityCollectible.cs
*\brief        AbilityCollectible.cs Script
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
    public unsafe class AbilityCollectible
    {
        private void* m_Address;
        UInt32 ID;
        public AbilityCollectible(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public bool m_Grow
        {
            get
            {
                return GetGrowStatus(ID);
            }
            set
            {
                SetGrowStatus(ID, value );
            }
        }

        public bool m_Shrink
        {
            get
            {
                return GetShrinkStatus(ID);
            }
            set
            {
                SetShrinkStatus(ID, value );
            }
        }

        public bool m_Freeze
        {
            get
            {
                return GetFreezeStatus(ID);
            }
            set
            {
                SetFreezeStatus(ID, value );
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetGrowStatus(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetShrinkStatus(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFreezeStatus(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetGrowStatus(UInt32 address, bool status);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetShrinkStatus(UInt32 address, bool status);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFreezeStatus(UInt32 address, bool status);
    }
}
