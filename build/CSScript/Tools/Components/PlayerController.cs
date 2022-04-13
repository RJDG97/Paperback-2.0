/**********************************************************************************
*\file         PlayerController.cs
*\brief        PlayerController.cs Script
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
    public unsafe class PlayerController
    {
        private void* m_Address;

        public PlayerController(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_FPSMode
        {
            get
            {
                return GetFPSMode(m_Address);
            }
            set 
            {
                SetFPSMode(m_Address, value);
            }
        }

        public bool m_GrowAvailable
        {
            get
            {
                return GetGrowStatus(m_Address);
            }
            set 
            {
                SetGrowStatus(m_Address, value);
            }
        }

        public bool m_ShrinkAvailable
        {
            get
            {
                return GetShrinkStatus(m_Address);
            }
            set 
            {
                SetShrinkStatus(m_Address, value);
            }
        }

        public bool m_FreezeAvailable
        {
            get
            {
                return GetFreezeStatus(m_Address);
            }
            set 
            {
                SetFreezeStatus(m_Address, value);
            }
        }

        public int m_CheckpointID
        {
            get
            {
                return GetCheckpointID(m_Address);
            }
            set
            {
                SetCheckpointID(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFPSMode(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFPSMode(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetGrowStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetGrowStatus(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetShrinkStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetShrinkStatus(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFreezeStatus(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFreezeStatus(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int GetCheckpointID(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetCheckpointID(void* address, int value);
    }
}
