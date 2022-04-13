/**********************************************************************************
*\file         ElevatorPlatform.cs
*\brief        ElevatorPlatform.cs Script
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
    public class ElevatorPlatform : MonoBehaviour
    {
        UInt32 m_ID;

        int m_JumpUnitID;
        int m_PushUnitID;

        Child m_Child;

        Transform m_Transform;
        Elevator m_Elevator;

        Sound m_Sound;
        Tools.MathLib.Vector3 m_PrevTransform;

        Transform JumpUnitTransform;
        Transform PushUnitTransform;

        bool JumpUnitUnder;
        bool PushUnitUnder;
        bool BlockUnder;

        public static ElevatorPlatform getInst()
        {
            return new ElevatorPlatform();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Transform = new Transform(m_ID);
             m_Child = new Child(m_ID);

            if (m_Child.m_ParentID != -1)
            {
                if (m_Child.m_ParentID != -1)
                {
                    m_Elevator = new Elevator((UInt32)m_Child.m_ParentID);
                    m_Sound = new Sound((UInt32)m_Child.m_ParentID);
                }
            }

            m_JumpUnitID = Player.GetJumpUnitID();
            m_PushUnitID = Player.GetPushUnitID();

            if (m_JumpUnitID != -1)
            {
                JumpUnitTransform = new Transform((UInt32)m_JumpUnitID);
            }

            if (m_PushUnitID != -1)
            {
                PushUnitTransform = new Transform((UInt32)m_PushUnitID);
            }
        }

        public void PreUpdate(float dt)
        {
            if (JumpUnitUnder || PushUnitUnder || BlockUnder)
            {
                m_Elevator.m_UnitUnder = true;
            }

            else
            {
                m_Elevator.m_UnitUnder = false;
            }

            Application.NotifyDone();
        }

        public void Update(float dt)
        {

            if (m_Transform.m_Position.y < m_PrevTransform.y || m_Transform.m_Position.y > m_PrevTransform.y)
            {

                m_Sound.m_Trigger = true;
            }
            else
            {

                Application.StopTaggedSoundComp(m_Sound.m_SoundPlayTag);
            }

            m_PrevTransform = m_Transform.m_Position;

            Application.NotifyDone();
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = JumpUnitTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    JumpUnitUnder = true;
                }
            }

            else if (ID == m_PushUnitID)
            {
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = PushUnitTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    PushUnitUnder = true;
                }
            }

            else if (Tools.Tag.IsPushable(ID))
            {
                Transform BlockTransform = new Transform((UInt32)ID);
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = BlockTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    BlockUnder = true;
                }
            }

            Application.NotifyDone();
        }
        public void OnCollisionStay(UInt32 ID)
        {
            Application.NotifyDone();
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                JumpUnitUnder = false;
            }

            else if (ID == m_PushUnitID)
            {
                PushUnitUnder = false;
            }

            else if (Tools.Tag.IsPushable(ID))
            {
                Transform BlockTransform = new Transform((UInt32)ID);
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = BlockTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    BlockUnder = false;
                }
            }

            Application.NotifyDone();
        }
        public void Reset()
        {
        }
    }
}
