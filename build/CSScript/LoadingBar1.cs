using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    class LoadingBar1
    {
        UInt32 m_ID;
        Scale m_Scale;
        float m_MaxBarScale;

        Transform m_pos;
        float m_InitPos;

        float m_Timer = 0.0f;
        float m_LoadingTime = 0.0f;
        Random m_rnd;

        public static LoadingBar1 getInst()
        {
            return new LoadingBar1();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Scale = new Scale(m_ID);
            m_MaxBarScale = m_Scale.m_Value.x;

            m_pos = new Transform(m_ID);
            m_InitPos = m_pos.m_Position.x;

            m_rnd = new Random();
            m_Timer = 0.0f;
            m_LoadingTime = (float)m_rnd.Next(1, 2);
            m_LoadingTime += (float)m_rnd.NextDouble();
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            m_Timer += dt;

            m_Scale.m_Value = new Tools.MathLib.Vector3((m_MaxBarScale * (m_Timer / (m_LoadingTime + 2.0f))), 
                m_Scale.m_Value.y, m_Scale.m_Value.z);

            m_pos.m_Position = new Tools.MathLib.Vector3((m_InitPos - (230.0f)) + ((230.0f) * (m_Timer / (m_LoadingTime + 2.0f))),
                m_pos.m_Position.y, m_pos.m_Position.z);

            if (m_Timer >= m_LoadingTime)
                Application.ChangeScene("LevelOne");
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
        }
    }
}
