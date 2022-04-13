/**********************************************************************************
*\file         Checkpoint.cs
*\brief        Checkpoint.cs Script
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
    public class Checkpoint : MonoBehaviour
    {
        UInt32 m_ID;
        Sound m_Sound; //empty game object is parent
        Parent m_Parent; //empty game object is parent
        Name m_Name;

        Int32 m_RedCPID;
        Int32 m_BlueCPID;
        Transform m_RedCPTransform;
        Transform m_BlueCPTransform;
        ParticleEmitter m_RedCPEmitter;
        ParticleEmitter m_BlueCPEmitter;

        Int32 m_JumpID;
        Int32 m_PushID;

        Transform m_JumpUnitTransform;
        Transform m_PushUnitTransform;
        Rigidforce m_JumpUnitRigidforce;
        Rigidforce m_PushUnitRigidforce;

        PlayerController m_JumpUnitController;
        PlayerController m_PushUnitController;

        bool m_Activated;

        public static Checkpoint getInst()
        {
            return new Checkpoint();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);
            m_Name = new Name(m_ID);

            m_RedCPID = m_Parent.GetChildIDofName("Red Checkpoint");
            m_BlueCPID = m_Parent.GetChildIDofName("Blue Checkpoint");

            if (m_RedCPID != -1 && m_BlueCPID != -1)
            {
                m_RedCPTransform = new Transform((UInt32)m_RedCPID);
                m_BlueCPTransform = new Transform((UInt32)m_BlueCPID);
                m_RedCPEmitter = new ParticleEmitter((UInt32)m_RedCPID);
                m_BlueCPEmitter = new ParticleEmitter((UInt32)m_BlueCPID);

                m_JumpID = Player.GetJumpUnitID();
                m_PushID = Player.GetPushUnitID();

                m_JumpUnitTransform = new Transform((UInt32)m_JumpID);
                m_PushUnitTransform = new Transform((UInt32)m_PushID);
                m_JumpUnitRigidforce = new Rigidforce((UInt32)m_JumpID);
                m_PushUnitRigidforce = new Rigidforce((UInt32)m_PushID);

                m_JumpUnitController = new PlayerController((UInt32)m_JumpID);
                m_PushUnitController = new PlayerController((UInt32)m_PushID);
            }

            m_Sound.m_Trigger = false;
            m_Activated = false;
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            Debug.Log("OnCollisionEnter Checkpoint: " + m_ID.ToString());

            if (!m_Activated && m_RedCPID != -1 && m_BlueCPID != -1)
            {
                if (ID == m_JumpID)
                {
                    m_Sound.m_Trigger = true;
                    m_PushUnitTransform.m_Position = m_RedCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                    m_PushUnitRigidforce.m_Momentum = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                    m_Activated = true;
                    m_JumpUnitController.m_CheckpointID = (int)m_ID;
                    m_PushUnitController.m_CheckpointID = (int)m_ID;
                }

                else if (ID == m_PushID)
                {
                    m_Sound.m_Trigger = true;
                    m_JumpUnitTransform.m_Position = m_BlueCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                    m_JumpUnitRigidforce.m_Momentum = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                    m_Activated = true;
                    m_JumpUnitController.m_CheckpointID = (int)m_ID;
                    m_PushUnitController.m_CheckpointID = (int)m_ID;
                }

                // Set Emitter Lifetime
                m_RedCPEmitter.m_Lifetime = 5.0f;
                m_BlueCPEmitter.m_Lifetime = 5.0f;
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
        }
        public void Reset()
        {
            if (m_JumpUnitController.m_CheckpointID == (int)m_ID)
            {
                m_Sound.m_Trigger = true;
                m_PushUnitTransform.m_Position = m_RedCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                m_PushUnitRigidforce.m_Momentum = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                m_JumpUnitTransform.m_Position = m_BlueCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                m_JumpUnitRigidforce.m_Momentum = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}
