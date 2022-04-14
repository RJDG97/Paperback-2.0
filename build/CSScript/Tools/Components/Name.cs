/**********************************************************************************
*\file         Name.cs
*\brief        Name.cs Script
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
    public unsafe class Name
    {
        private void* m_Address;

        public Name(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public String m_Name
        {
            get
            {
                return getname(m_Address);
            }
            set
            {
                setname(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getname(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setname(void* address, string value);
    }
}
