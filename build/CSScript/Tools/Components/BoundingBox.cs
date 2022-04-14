/**********************************************************************************
*\file         BoundingBox.cs
*\brief        BoundingBox.cs Script
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
    public unsafe class BoundingBox
    {
        UInt32 ID;
        public BoundingBox(UInt32 id)
        {
            ID = id;
        }

        public Tools.MathLib.Vector3 Min
        {
            get
            {
                return getmin(ID);
            }
            set
            {
                setmin(ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 Max
        {
            get
            {
                return getmax(ID);
            }
            set
            {
                setmax(ID, value.x, value.y, value.z);
            }
        }

        public bool m_Collided
        {
            get
            {
                return getcollided(ID);
            }
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmin(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmin(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmax(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmax(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getcollided(UInt32 ID);
    }
}
