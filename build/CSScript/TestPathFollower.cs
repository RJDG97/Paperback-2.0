using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class TestPathFollower : MonoBehaviour
    {
        UInt32 m_ID;
        PathFollower m_pathfollower;

        public static TestPathFollower getInst()
        {
            return new TestPathFollower();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_pathfollower = new PathFollower(m_ID);
        }
        public void Update(float dt)
        {
            if (Input.IsKeyPressDown(Input.PB_L))
            {
                m_pathfollower.m_PathID = 1;
                m_pathfollower.m_TravelSpeed = 50.0f;
                m_pathfollower.m_Reversed = !m_pathfollower.m_Reversed;
            }
        }
        public void Destroy()
        {
        }
    }
}
