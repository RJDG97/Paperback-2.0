/**********************************************************************************
*\file         GateSwitch2.cs
*\brief        GateSwitch2.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
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

namespace CSScript
{
    class GateSwitch2
    {
        UInt32 m_ID;
        Int32 m_ChildID;

        Sound m_Sound; //switch is parent
        Parent m_Parent; //switch is parent
        BoundingBox m_BoundingBox; //switch is parent
        Mesh m_Mesh;
        Rotation m_Rotation;

        Freezable m_ChildFreezable;
        BoundingBox m_ChildBoundingBox; //gate is child
        Animator m_ChildAnimator;
        Transform m_ChildTransform;

        Tools.MathLib.Vector3 m_InitialBoundingBoxMin;
        Tools.MathLib.Vector3 m_InitialBoundingBoxMax;
        Tools.MathLib.Vector3 m_InitialBBOffset;

        int m_NumTop = 0;
        bool m_Activated = false;

        public static GateSwitch2 getInst()
        {
            return new GateSwitch2();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);
            m_BoundingBox = new BoundingBox(m_ID);
            m_Mesh = new Mesh(m_ID);
            m_Rotation = new Rotation(m_ID);

            m_ChildID = m_Parent.GetChildIDofName("Gate");

            if (m_ChildID != -1)
            {
                m_ChildFreezable = new Freezable((UInt32)m_ChildID);
                m_ChildBoundingBox = new BoundingBox((UInt32)m_ChildID);
                m_ChildAnimator = new Animator((UInt32)m_ChildID);
                m_ChildTransform = new Transform((UInt32)m_ChildID);

                m_ChildAnimator.m_PauseAtTime = 0;
                m_InitialBoundingBoxMin = m_ChildBoundingBox.Min;
                m_InitialBoundingBoxMax = m_ChildBoundingBox.Max;
                m_InitialBBOffset = m_ChildTransform.m_Offset;
            }

            m_Sound.m_Trigger = false;
            m_Mesh.m_Model = "Button_GateOFF";
        }

        public void PreUpdate(float dt)
        {
            if ((m_Rotation.m_Value.y % 180.0f) < 1.0f && (m_Rotation.m_Value.y % 180.0f) > -1.0f)
            {
                m_ChildTransform.m_Offset = new Tools.MathLib.Vector3(m_InitialBBOffset.x - (m_ChildAnimator.m_CurrentTime / 48.0f * (m_InitialBoundingBoxMax.x - m_InitialBoundingBoxMin.x)),
                                                                  m_InitialBBOffset.y, m_InitialBBOffset.z);
            }

            else if ((m_Rotation.m_Value.y % 180.0f) < 91.0f && (m_Rotation.m_Value.y % 180.0f) > 89.0f)
            {
                m_ChildTransform.m_Offset = new Tools.MathLib.Vector3(m_InitialBBOffset.x,
                                                                      m_InitialBBOffset.y,
                                                                      m_InitialBBOffset.z - (m_ChildAnimator.m_CurrentTime / 48.0f * (m_InitialBoundingBoxMax.z - m_InitialBoundingBoxMin.z)));
            }

            if (m_NumTop == 0 && !m_Activated && !m_ChildFreezable.m_Frozen)
            {
                m_ChildAnimator.m_Reversed = true;
                m_ChildAnimator.m_PauseAnimation = false;
                m_ChildAnimator.m_PauseAtTime = 0;
            }

            Application.NotifyDone();
        }

        public void Update(float dt)
        {

            Application.NotifyDone();
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID)))
            {
                if (!m_ChildFreezable.m_Frozen)
                {
                    m_ChildAnimator.m_Reversed = false;
                    m_ChildAnimator.m_PauseAnimation = false;
                    m_ChildAnimator.m_PauseAtTime = 48;
                    ++m_NumTop;
                    m_Activated = true;
                }

                m_Sound.m_Trigger = true;
                m_Mesh.m_Model = "Button_GateON";
            }

            Application.NotifyDone();
        }

        public void OnCollisionStay(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID)))
            {
                if (!m_ChildFreezable.m_Frozen && !m_Activated)
                {
                    m_ChildAnimator.m_Reversed = false;
                    m_ChildAnimator.m_PauseAnimation = false;
                    m_ChildAnimator.m_PauseAtTime = 48;
                    ++m_NumTop;
                    m_Activated = true;
                }

                // m_Sound.m_Trigger = true;
                m_Mesh.m_Model = "Button_GateON";
            }

            Application.NotifyDone();
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID)))
            {
                --m_NumTop;
                m_Activated = false;
                m_Sound.m_Trigger = true;
                m_Mesh.m_Model = "Button_GateOFF";
            }

            Application.NotifyDone();
        }

        public void Reset()
        {
        }
    }
}
