using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MovingPlatformSwitch : MonoBehaviour
    {
        UInt32 m_ID;
        Parent m_Parent; //switch is parent
        Sound m_Sound;
        Mesh m_Mesh;

        Freezable m_ChildFreezable;
        PathFollower m_ChildPathFollower; //moving platform is child

        Int32 m_ChildID;
        Sound m_PlatformSound;
        int m_NumTop = 0;
        bool m_Activated = false;

        String m_OnModel;
        String m_OffModel;

        public static MovingPlatformSwitch getInst()
        {
            return new MovingPlatformSwitch();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Parent = new Parent(m_ID);
            m_Sound = new Sound(m_ID);
            m_Mesh = new Mesh(m_ID);

            m_ChildID = m_Parent.GetChildIDofName("Moving Platform");

            if (m_ChildID != -1)
            {
                m_ChildFreezable = new Freezable((UInt32)m_ChildID);
                m_ChildPathFollower = new PathFollower((UInt32)m_ChildID);
                m_PlatformSound = new Sound((UInt32)m_ChildID);

                m_ChildPathFollower.m_PauseTravel = true;
            }

            
            m_OffModel = m_Mesh.m_Model;
            m_OnModel = m_Mesh.m_Model.Substring(0, m_Mesh.m_Model.Length - 3) + "ON"; ;
        }

        public void PreUpdate(float dt)
        {
            if (m_ChildID != -1 && m_NumTop == 0 && !m_ChildFreezable.m_Frozen)
            {
                m_ChildPathFollower.m_Reversed = true;
                m_ChildPathFollower.m_PauseTravel = false;
                m_Activated = false;
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
            if (m_ChildID != -1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID()) || Tools.Tag.IsPushable(ID))
            {
                if (!m_ChildFreezable.m_Frozen)
                {
                    m_ChildPathFollower.m_PauseTravel = false;
                    m_ChildPathFollower.m_Reversed = false;
                    m_Sound.m_Trigger = true;
                    m_PlatformSound.m_Trigger = true;
                    ++m_NumTop;
                    m_Activated = true;
                }

                if (m_Mesh.m_Model == m_OffModel)
                {
                    m_Mesh.m_Model = m_OnModel;
                }
            }
        }

        public void OnCollisionStay(UInt32 ID)
        {
            if (m_ChildID != -1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID()) || Tools.Tag.IsPushable(ID))
            {
                if (!m_ChildFreezable.m_Frozen && !m_Activated)
                {
                    m_ChildPathFollower.m_PauseTravel = false;
                    m_ChildPathFollower.m_Reversed = false;
                    m_Sound.m_Trigger = true;
                    m_PlatformSound.m_Trigger = true;
                    ++m_NumTop;
                    m_Activated = true;
                }

                if (m_Mesh.m_Model == m_OffModel)
                {
                    m_Mesh.m_Model = m_OnModel;
                }
            }
        }

        public void OnCollisionExit(UInt32 ID)
        {
            if (m_ChildID != -1 && (ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID()) || Tools.Tag.IsPushable(ID))
            {
                if (m_Mesh.m_Model == m_OnModel)
                {
                    m_Mesh.m_Model = m_OffModel;
                }

                m_Sound.m_Trigger = true;
                --m_NumTop;
            }
        }
        public void Reset()
        {
        }
    }
}
