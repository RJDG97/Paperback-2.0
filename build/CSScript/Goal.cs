using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class Goal : MonoBehaviour
    {
        UInt32 m_ID;

        BoundingBox m_BoundingBox;
        public static Goal getInst()
        {
            return new Goal();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_BoundingBox = new BoundingBox(m_ID);
        }

        public void Update(float dt)
        {
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if ((ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() /*|| collision with blocks*/))
            {
                Application.ChangeScene("GameWin");
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
        }
    }
}
