using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MovingPlatform : MonoBehaviour
    {
        UInt32 m_ID;
        Transform m_transform;
        PathFollower m_pathfollower;

        public static MovingPlatform getInst()
        {
            return new MovingPlatform();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_transform = new Transform(m_ID);
            m_pathfollower = new PathFollower(m_ID);

            m_pathfollower.m_PauseTravel = true;
        }
        public void Update(float dt)
        {
            if (Input.IsMouseDown(Input.PB_MOUSE_BUTTON_1)) //replace if check with collision
            {
                m_pathfollower.m_PauseTravel = false;
            }

            else
            {
                m_pathfollower.m_PauseTravel = true;
            }
        }
        public void Destroy()
        {
        }
    }
}
