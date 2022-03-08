using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class PushableBlock : MonoBehaviour
    {
        UInt32 m_ID;
        Rigidforce m_Rigidforce;
        bool m_PrevGravActive;

        public static PushableBlock getInst()
        {
            return new PushableBlock();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Rigidforce = new Rigidforce(ID);
            m_PrevGravActive = m_Rigidforce.m_GravityActive;
        }

        public void Update(float dt)
        {
            if (m_PrevGravActive && !m_Rigidforce.m_GravityActive)
            {
                m_Rigidforce.m_CollisionAffected = false;
                m_Rigidforce.m_GravityAffected = false;
            }

            m_PrevGravActive = m_Rigidforce.m_GravityActive;
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
        }

        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
        }
    }
}
