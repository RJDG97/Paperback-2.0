/**********************************************************************************
*\file         MovingPlatform.cs
*\brief        MovingPlatform.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MovingPlatform : MonoBehaviour
    {
        UInt32 m_ID;
        Child m_Child;

        Transform m_ParentTransform;

        Tools.MathLib.Vector3 m_PrevPlatformPos;

        Int32 m_JumpUnitID;
        Int32 m_PushUnitID;

        Transform m_JumpUnitTransform;
        Transform m_PushUnitTransform;

        bool m_Starting;

        Sound m_ParentSound;

        int m_NumPassengers;

        public static MovingPlatform getInst()
        {
            return new MovingPlatform();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Child = new Child(m_ID);

            if (m_Child.m_ParentID != -1)
            {
                m_ParentTransform = new Transform((UInt32)m_Child.m_ParentID);
                m_ParentSound = new Sound((UInt32)m_Child.m_ParentID);
            }

            m_JumpUnitID = Player.GetJumpUnitID();

            if (m_JumpUnitID != -1)
            {
                m_JumpUnitTransform = new Transform((UInt32)m_JumpUnitID);
            }

            m_PushUnitID = Player.GetPushUnitID();

            if (m_PushUnitID != -1)
            {
                m_PushUnitTransform = new Transform((UInt32)m_PushUnitID);
            }

            m_Starting = false;

            m_NumPassengers = 0;
        }

        public void PreUpdate(float dt)
        {

            Application.NotifyDone();

        }

        public void Update(float dt)
        {
            if (m_Child.m_ParentID != -1)
            {
                if (m_NumPassengers > 0)
                {
                    m_NumPassengers = 0;
                }

                if ((m_PrevPlatformPos.x < m_ParentTransform.m_Position.x || m_PrevPlatformPos.x > m_ParentTransform.m_Position.x) ||
                    (m_PrevPlatformPos.z < m_ParentTransform.m_Position.z || m_PrevPlatformPos.z > m_ParentTransform.m_Position.z))
                {


                    if (m_Starting)
                    {

                        //Application.Trigger3DTaggedSound("SFX_Train_Loop", m_ParentTransform.m_Position.x, m_ParentTransform.m_Position.y, m_ParentTransform.m_Position.z, 0.0f, 0.0f, 0.0f, m_ID);
                        m_ParentSound.m_Trigger = true;
                    }

                    if (m_Starting == false)
                    {
                        //Application.Trigger3DTaggedSound("SFX_Train_Start", m_ParentTransform.m_Position.x, m_ParentTransform.m_Position.y, m_ParentTransform.m_Position.z, m_ParentRigidbody.m_Velocity.x, m_ParentRigidbody.m_Velocity.y, m_ParentRigidbody.m_Velocity.z, m_ID);
                        m_Starting = true;
                        //m_ParentSound.m_Trigger = true;
                    }
                }

                if ((m_PrevPlatformPos.x >= m_ParentTransform.m_Position.x - 0.01f && m_PrevPlatformPos.x <= m_ParentTransform.m_Position.x + 0.01f) &&
                    (m_PrevPlatformPos.z >= m_ParentTransform.m_Position.z - 0.01f && m_PrevPlatformPos.z <= m_ParentTransform.m_Position.z + 0.01f))
                {

                    m_Starting = false;

                    Application.StopTaggedSoundComp(m_ParentSound.m_SoundPlayTag);
                    //Application.Trigger3DTaggedSound("SFX_Train_Stop", m_ParentTransform.m_Position.x, m_ParentTransform.m_Position.y, m_ParentTransform.m_Position.z, m_ParentRigidbody.m_Velocity.x, m_ParentRigidbody.m_Velocity.y, m_ParentRigidbody.m_Velocity.z, m_ID);
                }

                m_PrevPlatformPos = m_ParentTransform.m_Position;
            }

            Application.NotifyDone();
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                ++m_NumPassengers;
            }

            else if (ID == m_PushUnitID)
            {
                ++m_NumPassengers;
            }

            else if (Tools.Tag.IsPushable(ID))
            {
                ++m_NumPassengers;
            }

            Application.NotifyDone();
        }

        public void OnCollisionStay(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                m_JumpUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                ++m_NumPassengers;
            }

            else if (ID == m_PushUnitID)
            {
                m_PushUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                ++m_NumPassengers;
            }

            else if (Tools.Tag.IsPushable(ID))
            {
                Transform box_transform = new Transform(ID);
                box_transform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                ++m_NumPassengers;
            }

            Application.NotifyDone();
        }

        public void OnCollisionExit(UInt32 ID)
        {

            Application.NotifyDone();

        }
        public void Reset()
        {
        }
    }
}

//using System;
//using System.Collections.Generic;
//using System.Text;
//using System.Threading.Tasks;

//namespace CSScript
//{
//    public class
//    : MonoBehaviour
//    {
//        UInt32 m_ID;
//        Child m_Child;

//        Transform m_ParentTransform;

//        Tools.MathLib.Vector3 m_PrevPlatformPos;

//        Int32 m_JumpUnitID;
//        Int32 m_PushUnitID;

//        Transform m_JumpUnitTransform;
//        Transform m_PushUnitTransform;

//        public static MovingPlatform getInst()
//        {
//            return new MovingPlatform();
//        }

//        public void Start(UInt32 ID)
//        {
//            m_ID = ID;
//            m_Child = new Child(m_ID);

//            if (m_Child.m_ParentID != -1)
//            {
//                m_ParentTransform = new Transform((UInt32)m_Child.m_ParentID);
//            }

//            m_JumpUnitID = Player.GetJumpUnitID();

//            if (m_JumpUnitID != -1)
//            {
//                m_JumpUnitTransform = new Transform((UInt32)m_JumpUnitID);
//            }

//            m_PushUnitID = Player.GetPushUnitID();

//            if (m_PushUnitID != -1)
//            {
//                m_PushUnitTransform = new Transform((UInt32)m_PushUnitID);
//            }
//        }
//        public void Update(float dt)
//        {
//        }
//        public void Destroy()
//        {
//        }

//        public void OnCollisionEnter(UInt32 ID)
//        {
//            m_PrevPlatformPos = m_ParentTransform.m_Position;
//        }

//        public void OnCollisionStay(UInt32 ID)
//        {
//            if (ID == m_JumpUnitID)
//            {
//                m_JumpUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
//                m_PrevPlatformPos = m_ParentTransform.m_Position;
//            }

//            else if (ID == m_PushUnitID)
//            {
//                m_PushUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
//                m_PrevPlatformPos = m_ParentTransform.m_Position;
//            }

//            else if (Tools.Tag.IsPushable(ID))
//            {
//                Transform box_transform = new Transform(ID);
//                box_transform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
//                m_PrevPlatformPos = m_ParentTransform.m_Position;
//            }
//        }

//        public void OnCollisionExit(UInt32 ID)
//        {

//        }
//    }
//}