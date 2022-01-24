using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class GateSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Sound m_Sound; //switch is parent
        Parent m_Parent; //switch is parent

        BoundingBox m_ChildBoundingBox; //gate is child
        Animator m_ChildAnimator;

        public static GateSwitch getInst()
        {
            return new GateSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);

            UInt32 m_ChildID = m_Parent.GetChildIDofName("Gate");

            m_ChildBoundingBox = new BoundingBox(m_ChildID);
            m_ChildAnimator = new Animator(m_ChildID);

            m_Sound.m_Trigger = false;
            m_ChildAnimator.m_PauseAnimation = true;
        }
        public void Update(float dt)
        {
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/)
            {
                m_Sound.m_Trigger = true;
                //m_ChildBoundingBox - toggle collidable
                m_ChildAnimator.m_PauseAnimation = false;
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
            if (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/)
            {
                m_ChildAnimator.m_PauseAnimation = true;
            }
        }
    }
}
