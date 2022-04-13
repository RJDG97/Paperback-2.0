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
        Mass m_Mass;
        Transform m_Transform;
        Pushable m_Pushable;
        ParticleEmitter m_ParticleEmitter;
        Tools.MathLib.Vector3 m_InitialPos;
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
            m_Pushable = new Pushable(ID);
            m_ParticleEmitter = new ParticleEmitter(ID);
            m_Pushable.m_State = 0;

            m_Mass = new Mass(ID);
            m_Transform = new Transform(ID);
            m_InitialPos = m_Transform.m_Position;
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            Debug.Log("Update, PusahbleBlock:" + m_ID.ToString());
            if (m_PrevGravActive && !m_Rigidforce.m_GravityActive)
            {
                m_Rigidforce.m_CollisionAffected = false;
                m_Rigidforce.m_GravityAffected = false;
                m_Mass.m_Mass = 0.0f;
            }

            m_PrevGravActive = m_Rigidforce.m_GravityActive;

            switch (m_Pushable.m_State)
            {
                case 1:  m_ParticleEmitter.m_Lifetime = 1.0f; m_ParticleEmitter.m_ParticleTexture = "GrowParticle";   break;
                case -1: m_ParticleEmitter.m_Lifetime = 1.0f; m_ParticleEmitter.m_ParticleTexture = "ShrinkParticle"; break;
                case 0:  m_ParticleEmitter.m_Lifetime = 0.0f; break;
            }
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
        public void Reset()
        {
            m_Transform.m_Position = m_InitialPos;
        }
    }
}
