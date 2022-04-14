/**********************************************************************************
*\file         Mesh.cs
*\brief        Mesh.cs Script
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
    public unsafe class Mesh
    {
        UInt32 ID;
        public Mesh(UInt32 id)
        {
            ID = id;
        }

        public string m_Model
        {
            get
            {
                return getmodel(ID);
            }
            set
            {
                setmodel(ID, value);
            }
        }

        public string m_Texture
        {
            get
            {
                return gettexture(ID);
            }
            set
            {
                settexture(ID, value);
            }
        }

        public int m_Bias
        {
            get
            {
                return getbias(ID);
            }
            set
            {
                setbias(ID, value);
            }
        }

        public bool m_Active
        {
            get
            {
                return getactive(ID);
            }
            set
            {
                setactive(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string getmodel(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmodel(UInt32 address, string model);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static string gettexture(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settexture(UInt32 address, string texture);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int getbias(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setbias(UInt32 address, int bias);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getactive(UInt32 address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setactive(UInt32 address, bool active);
    }
}
