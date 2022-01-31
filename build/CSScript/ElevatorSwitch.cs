using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class ElevatorSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        bool m_Retracting;
        Parent m_Parent; //switch is parent

        Animator m_ChildAnimator; //Elevator is child
        //Need its bounding box too, change offset when rising and retracting

        public static MovingPlatformSwitch getInst()
        {
            return new MovingPlatformSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Parent = new Parent(m_ID);
            m_ChildAnimator = new PathFollower(m_Parent.GetChildIDofName("Elevator"));
        }

        public void Update(float dt)
        {
            if (m_Retracting)
            {
                //move offset box up & scale it up
            }

            else
            {
                //move offset box down & scale it down
            }
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/)
            {
                m_Retracting = false;
                m_ChildAnimator.m_Reversed = false;
                //m_ChildAnimator.m_PauseAtFrame = something; //depends on what height is needed
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/)
            {
                m_Retracting = true;
                m_ChildAnimator.m_Reversed = true;
                m_ChildAnimator.m_PauseAtFrame = 0;
            }
        }
    }
}
