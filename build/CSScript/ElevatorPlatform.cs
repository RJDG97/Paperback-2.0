using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class ElevatorPlatform : MonoBehaviour
    {
        UInt32 m_ID;

        int m_JumpUnitID;
        int m_PushUnitID;

        Transform m_Transform;
        Elevator m_Elevator;

        Transform JumpUnitTransform;
        Transform PushUnitTransform;

        bool JumpUnitUnder;
        bool PushUnitUnder;

        public static ElevatorPlatform getInst()
        {
            return new ElevatorPlatform();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Transform = new Transform(m_ID);
            Child m_Child = new Child(m_ID);

            if (m_Child.m_ParentID != -1)
            {
                Parent parent = new Parent((UInt32)m_Child.m_ParentID);
                int m_ElevatorID = parent.GetChildIDofName("Elevator");

                if (m_ElevatorID != -1)
                {
                    m_Elevator = new Elevator((UInt32)m_ElevatorID);
                }
            }

            m_JumpUnitID = Player.GetJumpUnitID();
            m_PushUnitID = Player.GetPushUnitID();

            if (m_JumpUnitID != -1)
            {
                JumpUnitTransform = new Transform((UInt32)m_JumpUnitID);
            }

            if (m_PushUnitID != -1)
            {
                PushUnitTransform = new Transform((UInt32)m_PushUnitID);
            }
        }

        public void PreUpdate(float dt)
        {
            if (JumpUnitUnder || PushUnitUnder)
            {
                m_Elevator.m_UnitUnder = true;
            }

            else
            {
                m_Elevator.m_UnitUnder = false;
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
            if (ID == m_JumpUnitID)
            {
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = JumpUnitTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    JumpUnitUnder = true;
                }
            }

            else if (ID == m_PushUnitID)
            {
                Tools.MathLib.Vector3 down_vec = new Tools.MathLib.Vector3(0.0f, -1.0f, 0.0f);
                Tools.MathLib.Vector3 dir_vec = PushUnitTransform.m_Position - m_Transform.m_Position;

                float dot_prod = dir_vec.y * down_vec.y;

                if (dot_prod > 0.0f)
                {
                    PushUnitUnder = true;
                }
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (ID == m_JumpUnitID)
            {
                JumpUnitUnder = false;
            }

            else if (ID == m_PushUnitID)
            {
                PushUnitUnder = false;
            }
        }
    }
}
