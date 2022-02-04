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

        Int32 m_RedCPID;
        Int32 m_BlueCPID;
        Transform m_RedCPTransform;
        Transform m_BlueCPTransform;

        Int32 m_JumpID;
        Int32 m_PushID;

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

            m_RedCPID = m_Parent.GetChildIDofName("Red Checkpoint");
            m_BlueCPID = m_Parent.GetChildIDofName("Blue Checkpoint");

            if (m_RedCPID != -1 && m_BlueCPID != -1)
            {
                m_RedCPTransform = new Transform((UInt32)m_RedCPID);
                m_BlueCPTransform = new Transform((UInt32)m_BlueCPID);

                m_JumpID = Player.GetJumpUnitID();
                m_PushID = Player.GetPushUnitID();

                m_JumpUnitTransform = new Transform((UInt32)m_JumpID);
                m_PushUnitTransform = new Transform((UInt32)m_PushID);
            }

            m_Sound.m_Trigger = false;
            m_Activated = false;
        }
        public void Update(float dt)
        {
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (!m_Activated && m_RedCPID != -1 && m_BlueCPID != -1)
            {
                if (ID == m_JumpID || ID == m_PushID)
                {
                    m_Sound.m_Trigger = true;
                    m_PushUnitTransform.m_Position = m_RedCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                    m_JumpUnitTransform.m_Position = m_BlueCPTransform.m_Position + new Tools.MathLib.Vector3(0.0f, 0.2f, 0.0f);
                    m_Activated = true;
                }
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
