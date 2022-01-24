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

        Transform m_RedRoboTransform;
        Transform m_BlueRoboTransform;

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

            UInt32 m_RedID = m_Parent.GetChildIDofName("Red Checkpoint");
            UInt32 m_BlueID = m_Parent.GetChildIDofName("Red Checkpoint");

            m_RedCPBoundingBox = new BoundingBox(m_RedID);
            m_BlueCPBoundingBox = new BoundingBox(m_BlueID);
            m_RedCPTransform = new Transform(m_RedID);
            m_BlueCPTransform = new Transform(m_BlueID);

            m_RedRoboTransform = new Transform(Player.GetRedRobotID());
            m_BlueRoboTransform = new Transform(Player.GetBlueRobotID());

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
                if (ID == Player.GetRedRobotID())
                {
                    m_Sound.m_Trigger = true;
                    m_RedRoboTransform.m_Position = m_BlueCPTransform.m_Position - new Tools.MathLib.Vector3(0.0f, 1.0f, 0.0f);
                }

                else if (ID == Player.GetBlueRobotID())
                {
                    m_Sound.m_Trigger = true;
                    m_BlueRoboTransform.m_Position = m_RedCPTransform.m_Position - new Tools.MathLib.Vector3(0.0f, 1.0f, 0.0f);
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
