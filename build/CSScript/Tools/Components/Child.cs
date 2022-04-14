/**********************************************************************************
*\file         Child.cs
*\brief        Child.cs Script
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
    public unsafe class Child
    {
        private void* m_Address;
        UInt32 ID;

        public Child(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public UInt32 m_ParentID
        {
            get
            {
                return getparentid(ID);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static UInt32 getparentid(UInt32 ID);
    }
}
