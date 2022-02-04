using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class Checkpoint : MonoBehaviour
    {
        UInt32 m_ID;
        Sound m_Sound; //empty game object is parent
        Parent m_Parent; //empty game object is parent

        BoundingBox m_RedCPBoundingBox; //red checkpoint is child
        BoundingBox m_BlueCPBoundingBox; //blue checkpoint is also child
        Transform m_RedCPTransform;
        Transform m_BlueCPTransform;

        Transform m_JumpUnitTransform;
        Transform m_PushUnitTransform;

        bool m_Activated;

        public static GateSwitch getInst()
        {
            return new GateSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);

            Int32 m_RedID = m_Parent.GetChildIDofName("Red Checkpoint");
            Int32 m_BlueID = m_Parent.GetChildIDofName("Blue Checkpoint");

            m_RedCPBoundingBox = new BoundingBox((UInt32)m_RedID);
            m_BlueCPBoundingBox = new BoundingBox((UInt32)m_BlueID);
            m_RedCPTransform = new Transform((UInt32)m_RedID);
            m_BlueCPTransform = new Transform((UInt32)m_BlueID);

            m_JumpUnitTransform = new Transform((UInt32)Player.GetJumpUnitID());
            m_PushUnitTransform = new Transform((UInt32)Player.GetPushUnitID());

            m_Sound.m_Trigger = false;
        }
        public void Update(float dt)
        {
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (!m_Activated)
            {
                if (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID())
                {
                    m_Sound.m_Trigger = true;
                    m_PushUnitTransform.m_Position = m_RedCPTransform.m_Position - new Tools.MathLib.Vector3(0.0f, 1.0f, 0.0f);
                    m_JumpUnitTransform.m_Position = m_BlueCPTransform.m_Position - new Tools.MathLib.Vector3(0.0f, 1.0f, 0.0f);
                }

                m_Activated = true;
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
