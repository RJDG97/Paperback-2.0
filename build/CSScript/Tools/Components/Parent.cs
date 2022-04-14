/**********************************************************************************
*\file         Parent.cs
*\brief        Parent.cs Script
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
    public unsafe class Parent
    {
        private void* m_Address;
        UInt32 ID;
        public Parent(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public Int32 GetChildIDofName(String child_name)
        {
            return getchildidofname(ID, child_name);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Int32 getchildidofname(UInt32 ID, string name);
    }
}
