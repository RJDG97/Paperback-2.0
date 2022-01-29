using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class GateSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Int32 m_ChildID;

        Sound m_Sound; //switch is parent
        Parent m_Parent; //switch is parent
        BoundingBox m_BoundingBox; //switch is parent

        BoundingBox m_ChildBoundingBox; //gate is child
        Animator m_ChildAnimator;

        Rigidbody m_Rigidbody;
        Rigidforce m_Rigidforce;

        public static GateSwitch getInst()
        {
            return new GateSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Sound = new Sound(m_ID);
            m_Parent = new Parent(m_ID);
            m_BoundingBox = new BoundingBox(m_ID);

            m_ChildID = m_Parent.GetChildIDofName("Gate");

            if (m_ChildID != -1)
            {
                m_ChildBoundingBox = new BoundingBox((UInt32)m_ChildID);
                m_ChildAnimator = new Animator((UInt32)m_ChildID);
                m_ChildAnimator.m_PauseAnimation = true;
            }

            m_Rigidbody = new Rigidbody(m_ID);
            m_Rigidforce = new Rigidforce(m_ID);

            m_Sound.m_Trigger = false;
        }

        public void Update(float dt)
        {
            if (Input.IsKeyPressDown(Input.PB_G))
                m_Rigidbody.m_Velocity += new Tools.MathLib.Vector3(5f, 5f, 5f);

            if (Input.IsKeyPressDown(Input.PB_H))
                m_Rigidforce.m_Momentum += new Tools.MathLib.Vector3(5f, 5f, 5f);
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/))
            {
                m_Sound.m_Trigger = true;
                //m_ChildBoundingBox - toggle collidable
                m_ChildAnimator.m_PauseAnimation = false;
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetRedRobotID() || ID == Player.GetBlueRobotID() /*|| collision with blocks*/))
            {
                m_ChildAnimator.m_PauseAnimation = true;
            }
        }
    }
}
