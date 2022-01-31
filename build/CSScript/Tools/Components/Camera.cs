﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Camera
    {
        private void* m_Address;
        private UInt32 m_ID;

        public Camera(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_Active
        {
            get
            {
                return getactive(m_Address);
            }
            set
            {
                setactive(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactive(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactive(void* address, bool mass);
    }
}