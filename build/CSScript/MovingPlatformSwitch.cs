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

        PathFollower m_ChildPathFollower; //moving platform is child

        public static MovingPlatformSwitch getInst()
        {
            return new MovingPlatformSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Parent = new Parent(m_ID);
            m_ChildPathFollower = new PathFollower(m_Parent.m_ChildID);
            m_ChildPathFollower.m_PauseTravel = true;
        }
        public void Update(float dt)
        {
            if (Input.IsMouseDown(Input.PB_MOUSE_BUTTON_1)) //replace if check with collision
            {
                m_ChildPathFollower.m_PauseTravel = false;
            }

            else
            {
                m_ChildPathFollower.m_PauseTravel = true;
            }
        }
        public void Destroy()
        {
        }
    }
}
