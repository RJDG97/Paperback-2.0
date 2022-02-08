using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MovingPlatform : MonoBehaviour
    {
        UInt32 m_ID;
        Child m_Child;

        Transform m_ParentTransform;

        Tools.MathLib.Vector3 m_PrevPlatformPos;

        Int32 m_JumpUnitID;
        Int32 m_PushUnitID;

        Transform m_JumpUnitTransform;
        Transform m_PushUnitTransform;

        public static MovingPlatform getInst()
        {
            return new MovingPlatform();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Child = new Child(m_ID);

            if (m_Child.m_ParentID != -1)
            {
                m_ParentTransform = new Transform((UInt32)m_Child.m_ParentID);
            }

            m_JumpUnitID = Player.GetJumpUnitID();

            if (m_JumpUnitID != -1)
            {
                m_JumpUnitTransform = new Transform((UInt32)m_JumpUnitID);
            }

            m_PushUnitID = Player.GetPushUnitID();

            if (m_PushUnitID != -1)
            {
                m_PushUnitTransform = new Transform((UInt32)m_PushUnitID);
            }
        }
        public void Update(float dt)
        {
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            m_PrevPlatformPos = m_ParentTransform.m_Position;
        }

        public void OnCollisionStay(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                m_JumpUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                m_PrevPlatformPos = m_ParentTransform.m_Position;
            }

            else if (ID == m_PushUnitID)
            {
                m_PushUnitTransform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                m_PrevPlatformPos = m_ParentTransform.m_Position;
            }

            else if (Tools.Tag.IsPushable(ID))
            {
                Transform box_transform = new Transform(ID);
                box_transform.m_Position += m_ParentTransform.m_Position - m_PrevPlatformPos;
                m_PrevPlatformPos = m_ParentTransform.m_Position;
            }
        }

        public void OnCollisionExit(UInt32 ID)
        {

        }
    }
}
