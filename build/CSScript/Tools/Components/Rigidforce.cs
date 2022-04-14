/**********************************************************************************
*\file         Rigidforce.cs
*\brief        Rigidforce.cs Script
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
    public unsafe class Rigidforce
    {
        UInt32 m_ID;

        public Rigidforce(UInt32 id)
        {
            m_ID = id;
        }

        public float m_StaticFriction
        {
            get
            {
                return getstaticfriction(m_ID);
            }
            set
            {
                setstaticfriction(m_ID, value);
            }
        }

        public float m_DynamicFriction
        {
            get
            {
                return getdynamicfriction(m_ID);
            }
            set
            {
                setdynamicfriction(m_ID, value);
            }
        }

        public Tools.MathLib.Vector3 m_Forces
        {
            get
            {
                return getforces(m_ID);
            }
            set
            {
                setforces(m_ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Momentum
        {
            get
            {
                return getmomentum(m_ID);
            }
            set
            {
                setmomentum(m_ID, value.x, value.y, value.z);
            }
        }

        public float m_Restitution
        {
            get
            {
                return getrestitution(m_ID);
            }
            set
            {
                setrestitution(m_ID, value);
            }
        }

        public bool m_CollisionAffected
        {
            get
            {
                return getcollisionaffected(m_ID);
            }
            set
            {
                setcollisionaffected(m_ID, value);
            }
        }

        public bool m_GravityAffected
        {
            get
            {
                return getgravityaffected(m_ID);
            }
            set
            {
                setgravityaffected(m_ID, value);
            }
        }
        public bool m_GravityActive
        {
            get
            {
                return getgravityactive(m_ID);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstaticfriction(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstaticfriction(UInt32 ID, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getdynamicfriction(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdynamicfriction(UInt32 ID, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getforces(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setforces(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmomentum(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmomentum(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getrestitution(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setrestitution(UInt32 ID, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getgravityaffected(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setgravityaffected(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getcollisionaffected(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcollisionaffected(UInt32 ID, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getgravityactive(UInt32 ID);
    }
}
