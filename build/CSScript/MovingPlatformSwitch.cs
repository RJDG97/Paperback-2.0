using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MovingPlatformSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Parent m_Parent; //switch is parent
        Sound m_Sound;

        PathFollower m_ChildPathFollower; //moving platform is child

        Int32 m_ChildID;
        Sound m_PlatformSound;

        public static MovingPlatformSwitch getInst()
        {
            return new MovingPlatformSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Parent = new Parent(m_ID);
            m_Sound = new Sound(m_ID);

            m_ChildID = m_Parent.GetChildIDofName("Moving Platform");

            if (m_ChildID != -1)
            {
                m_ChildPathFollower = new PathFollower((UInt32)m_ChildID);
                m_PlatformSound = new Sound((UInt32)m_ChildID);
            }

            m_ChildPathFollower.m_PauseTravel = true;
        }
        public void Update(float dt)
        {
            
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (m_ChildID != -1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID()) || Tools.Tag.IsPushable(ID))
            {
                m_ChildPathFollower.m_PauseTravel = false;
                m_ChildPathFollower.m_Reversed = false;
                m_Sound.m_Trigger = true;
                //m_PlatformSound.m_Trigger = true;
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
            if (m_ChildID != -1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID()) || Tools.Tag.IsPushable(ID))
            {
                m_ChildPathFollower.m_Reversed = true;
            }
        }
    }
}
