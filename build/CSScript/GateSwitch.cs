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
        Mesh m_Mesh;

        BoundingBox m_ChildBoundingBox; //gate is child
        Animator m_ChildAnimator;

        Tools.MathLib.Vector3 m_InitialBoundingBoxMin;
        Tools.MathLib.Vector3 m_InitialBoundingBoxMax;

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
            m_Mesh = new Mesh(m_ID);

            m_ChildID = m_Parent.GetChildIDofName("Gate");

            if (m_ChildID != -1)
            {
                m_ChildBoundingBox = new BoundingBox((UInt32)m_ChildID);
                m_ChildAnimator = new Animator((UInt32)m_ChildID);
            }

            m_Sound.m_Trigger = false;

            m_InitialBoundingBoxMin = m_ChildBoundingBox.Min;
            m_InitialBoundingBoxMax = m_ChildBoundingBox.Max;

            m_ChildAnimator.m_PauseAtTime = 0;
            m_Mesh.m_Model = "Button_GateOFF";
        }

        public void Update(float dt)
        {
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID)))
            {
                m_Sound.m_Trigger = true;
                m_ChildBoundingBox.Min = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                m_ChildBoundingBox.Max = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                m_ChildAnimator.m_Reversed = false;
                m_ChildAnimator.m_PauseAnimation = false;
                m_ChildAnimator.m_PauseAtTime = 74;
                m_Mesh.m_Model = "Button_GateON";
            }
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
            if (m_ChildID != 1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() || Tools.Tag.IsPushable(ID)))
            {
                m_ChildBoundingBox.Min = m_InitialBoundingBoxMin;
                m_ChildBoundingBox.Max = m_InitialBoundingBoxMax;
                m_ChildAnimator.m_Reversed = true;
                m_ChildAnimator.m_PauseAnimation = false;
                m_ChildAnimator.m_PauseAtTime = 0;
                m_Mesh.m_Model = "Button_GateOFF";
            }
        }
    }
}
