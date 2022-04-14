/**********************************************************************************
*\file         ElevatorSwitch.cs
*\brief        ElevatorSwitch.cs Script
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
    public class ElevatorSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Sound m_Sound; //switch is parent
        Parent m_Parent; //switch is parent

        Int32 m_ElevatorID;
        Mesh m_Mesh;
        Freezable m_ElevatorFreezable;
        Animator m_ElevatorAnimator;
        Elevator m_ElevatorElevator;
        Sound m_ElevatorSound;
        Scale m_ElevatorScale;

        ParticleEmitter m_PlatformSlopeAEmitter;

        Int32 m_PlatformID;
        Offset m_PlatformOffset;

        float m_PlatformStartY;
        float m_PlatformSpeed;

        bool m_GoingUpwards;
        int m_NumTop = 0;

        bool m_Activated = false;

        public static ElevatorSwitch getInst()
        {
            return new ElevatorSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);
            m_Mesh = new Mesh((UInt32)m_ID);

            m_ElevatorID = m_Parent.GetChildIDofName("Elevator");

            if (m_ElevatorID != -1)
            {
                m_ElevatorFreezable = new Freezable((UInt32)m_ElevatorID);
                m_ElevatorAnimator = new Animator((UInt32)m_ElevatorID);
                m_ElevatorElevator = new Elevator((UInt32)m_ElevatorID);
                m_ElevatorScale = new Scale((UInt32)m_ElevatorID);
                m_ElevatorSound = new Sound((UInt32)m_ElevatorID);

                Parent ElevatorParent = new Parent((UInt32)m_ElevatorID);
                m_PlatformID = ElevatorParent.GetChildIDofName("Platform");

                if (m_PlatformID != -1)
                {
                    m_PlatformOffset = new Offset((UInt32)m_PlatformID);
                    m_PlatformStartY = m_PlatformOffset.m_PosOffset.y;

                    Parent PlatformParent = new Parent((UInt32)m_PlatformID);
                    Int32 platform_slope_a_id = PlatformParent.GetChildIDofName("PlatformSlopeA");
                    m_PlatformSlopeAEmitter = new ParticleEmitter((UInt32)platform_slope_a_id);
                }
            }

            m_ElevatorAnimator.m_CurrentTime = m_ElevatorElevator.m_StartTime;
            m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StartTime;
            m_PlatformSpeed = 1800.0f * m_ElevatorScale.m_Value.y;
            m_GoingUpwards = false;
        }

        public void PreUpdate(float dt)
        {
            if (m_Mesh != null && m_ElevatorFreezable != null &&
                m_ElevatorAnimator != null && m_ElevatorElevator != null &&
                m_ElevatorSound != null && m_ElevatorScale != null &&
                m_PlatformSlopeAEmitter != null && m_PlatformOffset != null)
            {
                if (m_ElevatorID != -1)
                {
                    m_PlatformOffset.m_PosOffset = new Tools.MathLib.Vector3(m_PlatformOffset.m_PosOffset.x,
                                                                         m_PlatformStartY + m_ElevatorAnimator.m_CurrentTime / 48.0f * m_PlatformSpeed,
                                                                         m_PlatformOffset.m_PosOffset.z);

                    if (m_ElevatorElevator.m_UnitUnder && !m_GoingUpwards)
                    {
                        m_ElevatorAnimator.m_PauseAnimation = true;
                    }

                    else
                    {
                        m_ElevatorAnimator.m_PauseAnimation = false;
                    }

                    if (!m_ElevatorFreezable.m_Frozen)
                    {
                        if (m_NumTop == 0)
                        {
                            m_ElevatorAnimator.m_PauseAnimation = false;

                            if (m_ElevatorElevator.m_StartTime < m_ElevatorElevator.m_StopTime)
                            {
                                m_ElevatorAnimator.m_Reversed = true;
                                m_GoingUpwards = false;
                            }

                            else
                            {
                                m_ElevatorAnimator.m_Reversed = false;
                                m_GoingUpwards = true;
                            }

                            m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StartTime;
                            m_Activated = false;
                        }

                        m_PlatformSlopeAEmitter.m_Lifetime = 0.0f;
                    }

                    else
                    {
                        m_PlatformSlopeAEmitter.m_Lifetime = 0.5f;
                    }
                }
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
            if (m_Mesh != null && m_ElevatorFreezable != null &&
                m_ElevatorAnimator != null && m_ElevatorElevator != null &&
                m_ElevatorSound != null && m_ElevatorScale != null &&
                m_PlatformSlopeAEmitter != null && m_PlatformOffset != null)
            {
                if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID))
                {
                    if (!m_ElevatorFreezable.m_Frozen)
                    {
                        m_ElevatorSound.m_Trigger = true;
                        m_ElevatorAnimator.m_PauseAnimation = false;

                        if (m_ElevatorElevator.m_StartTime < m_ElevatorElevator.m_StopTime)
                        {
                            m_ElevatorAnimator.m_Reversed = false;
                            m_GoingUpwards = true;
                        }

                        else
                        {
                            m_ElevatorAnimator.m_Reversed = true;
                            m_GoingUpwards = false;
                        }

                        m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StopTime;
                        ++m_NumTop;

                        m_Activated = true;
                    }

                    m_Sound.m_Trigger = true;

                    if (m_Mesh.m_Model.Substring(m_Mesh.m_Model.Length - 3, 3) == "OFF")
                    {
                        m_Mesh.m_Model = m_Mesh.m_Model.Substring(0, m_Mesh.m_Model.Length - 3) + "ON";
                    }
                }
            }

            Application.NotifyDone();
        }

        public void OnCollisionStay(UInt32 ID)
        {

            if (m_Mesh != null && m_ElevatorFreezable != null &&
                m_ElevatorAnimator != null && m_ElevatorElevator != null &&
                m_ElevatorSound != null && m_ElevatorScale != null &&
                m_PlatformSlopeAEmitter != null && m_PlatformOffset != null)
            {
                if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID))
                {
                    if (!m_ElevatorFreezable.m_Frozen && !m_Activated)
                    {
                        m_ElevatorSound.m_Trigger = true;
                        m_ElevatorAnimator.m_PauseAnimation = false;

                        if (m_ElevatorElevator.m_StartTime < m_ElevatorElevator.m_StopTime)
                        {
                            m_ElevatorAnimator.m_Reversed = false;
                            m_GoingUpwards = true;
                        }

                        else
                        {
                            m_ElevatorAnimator.m_Reversed = true;
                            m_GoingUpwards = false;
                        }

                        m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StopTime;
                        ++m_NumTop;

                        m_Activated = true;
                    }

                    // m_Sound.m_Trigger = true;

                    if (m_Mesh.m_Model.Substring(m_Mesh.m_Model.Length - 3, 3) == "OFF")
                    {
                        m_Mesh.m_Model = m_Mesh.m_Model.Substring(0, m_Mesh.m_Model.Length - 3) + "ON";
                    }
                }
            }

            Application.NotifyDone();
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (m_Mesh != null && m_ElevatorFreezable != null &&
                m_ElevatorAnimator != null && m_ElevatorElevator != null &&
                m_ElevatorSound != null && m_ElevatorScale != null &&
                m_PlatformSlopeAEmitter != null && m_PlatformOffset != null)
            {
                if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID))
                {
                    if (m_Mesh.m_Model.Substring(m_Mesh.m_Model.Length - 2, 2) == "ON")
                    {
                        m_Mesh.m_Model = m_Mesh.m_Model.Substring(0, m_Mesh.m_Model.Length - 2) + "OFF";
                    }

                    --m_NumTop;
                    m_Sound.m_Trigger = true;
                }
            }

            Application.NotifyDone();
        }

        public void Reset()
        {
        }
    }
}
