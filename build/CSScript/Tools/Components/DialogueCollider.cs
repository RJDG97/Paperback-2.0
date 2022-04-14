/**********************************************************************************
*\file         DialogueCollider.cs
*\brief        DialogueCollider.cs Script
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
    public unsafe class DialogueCollider
    {
        UInt32 ID;
        public DialogueCollider(UInt32 id)
        {
            ID = id;
        }

        public String m_DialogueName
        {
            get
            {
                return getdialoguename(ID);
            }
            set
            {
                setdialoguename(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static String getdialoguename(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdialoguename(UInt32 address, String value);
    }
}
