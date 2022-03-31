﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class PushableBlock : MonoBehaviour
    {
        UInt32 m_ID;
        Rigidforce m_Rigidforce;
        Mass m_Mass;
        Transform m_Transform;
        Pushable m_Pushable;
        Tools.MathLib.Vector3 m_InitialPos;
        bool m_PrevGravActive;

        public static PushableBlock getInst()
        {
            return new PushableBlock();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Rigidforce = new Rigidforce(ID);
            m_PrevGravActive = m_Rigidforce.m_GravityActive;
            m_Pushable = new Pushable(ID);
            m_Pushable.m_State = 0;

            m_Mass = new Mass(ID);
            m_Transform = new Transform(ID);
            m_InitialPos = m_Transform.m_Position;
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            if (m_PrevGravActive && !m_Rigidforce.m_GravityActive)
            {
                m_Rigidforce.m_CollisionAffected = false;
                m_Rigidforce.m_GravityAffected = false;
                m_Mass.m_Mass = 0.0f;
            }

            m_PrevGravActive = m_Rigidforce.m_GravityActive;
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
        }

        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
        }
        public void Reset()
        {
            m_Transform.m_Position = m_InitialPos;
        }
    }
}
