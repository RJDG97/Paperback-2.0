using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class ElevatorSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Parent m_Parent; //switch is parent

        Int32 m_ElevatorID;
        Animator m_ElevatorAnimator;
        Elevator m_ElevatorElevator;
        Scale m_ElevatorScale;

        Int32 m_PlatformID;
        Offset m_PlatformOffset;

        float m_PlatformStartY;
        float m_PlatformSpeed;

        public static ElevatorSwitch getInst()
        {
            return new ElevatorSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Parent = new Parent(m_ID);

            m_ElevatorID = m_Parent.GetChildIDofName("Elevator");

            if (m_ElevatorID != -1)
            {
                m_ElevatorAnimator = new Animator((UInt32)m_ElevatorID);
                m_ElevatorElevator = new Elevator((UInt32)m_ElevatorID);
                m_ElevatorScale = new Scale((UInt32)m_ElevatorID);
            }

            m_PlatformID = m_Parent.GetChildIDofName("Platform");

            if (m_PlatformID != -1)
            {
                m_PlatformOffset = new Offset((UInt32)m_PlatformID);
                m_PlatformStartY = m_PlatformOffset.m_PosOffset.y;
            }

            m_ElevatorAnimator.m_CurrentTime = m_ElevatorElevator.m_StartTime;
            m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StartTime;
            m_PlatformSpeed = 1800.0f * m_ElevatorScale.m_Value.y;
        }

        public void Update(float dt)
        {
            if (!m_ElevatorAnimator.m_PauseAnimation) //Not paused, need to continue moving the bounding box
            {
                m_PlatformOffset.m_PosOffset = new Tools.MathLib.Vector3(m_PlatformOffset.m_PosOffset.x ,
                                                                         m_PlatformStartY + m_ElevatorAnimator.m_CurrentTime / 48.0001f * m_PlatformSpeed,
                                                                         m_PlatformOffset.m_PosOffset.z);
            }
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() /*|| collision with blocks*/)
            {
                m_ElevatorAnimator.m_PauseAnimation = false;

                if (m_ElevatorElevator.m_StartTime < m_ElevatorElevator.m_StopTime)
                {
                    m_ElevatorAnimator.m_Reversed = false;
                }

                else
                {
                    m_ElevatorAnimator.m_Reversed = true;
                }

                m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StopTime;
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() /*|| collision with blocks*/)
            {
                m_ElevatorAnimator.m_PauseAnimation = false;

                if (m_ElevatorElevator.m_StartTime < m_ElevatorElevator.m_StopTime)
                {
                    m_ElevatorAnimator.m_Reversed = true;
                }

                else
                {
                    m_ElevatorAnimator.m_Reversed = false;
                }

                m_ElevatorAnimator.m_PauseAtTime = m_ElevatorElevator.m_StartTime;
            }
        }
    }
}
