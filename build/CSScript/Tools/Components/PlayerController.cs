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
        UInt32 ID;

        public PlayerController(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public bool m_FPSMode
        {
            get
            {
                return GetFPSMode(ID);
            }
            set 
            {
                SetFPSMode(ID, value);
            }
        }

        public bool m_GrowAvailable
        {
            get
            {
                return GetGrowStatus(ID);
            }
            set 
            {
                SetGrowStatus(ID, value);
            }
        }

        public bool m_ShrinkAvailable
        {
            get
            {
                return GetShrinkStatus(ID);
            }
            set 
            {
                SetShrinkStatus(ID, value);
            }
        }

        public bool m_FreezeAvailable
        {
            get
            {
                return GetFreezeStatus(ID);
            }
            set 
            {
                SetFreezeStatus(ID, value);
            }
        }

        public int m_CheckpointID
        {
            get
            {
                return GetCheckpointID(ID);
            }
            set
            {
                SetCheckpointID(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFPSMode(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFPSMode(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetGrowStatus(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetGrowStatus(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetShrinkStatus(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetShrinkStatus(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFreezeStatus(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFreezeStatus(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int GetCheckpointID(UInt32 IDs);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetCheckpointID(UInt32 ID, int value);
    }
}
