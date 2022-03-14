using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    class AbilitySpin
    {
        UInt32 m_ID;
        Rotation m_Rotation;
        Sound m_Sound;
        Tools.MathLib.Vector3 m_Speed;

        public static AbilitySpin getInst()
        {
            return new AbilitySpin();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Rotation = new Rotation(m_ID);
            m_Sound = new Sound(m_ID);
            m_Speed = new Tools.MathLib.Vector3(0.0f, 2.0f, 0.0f);
        }
        public void Update(float dt)
        {
            m_Rotation.m_Value += m_Speed;
            if (m_Rotation.m_Value.y >= 360)
            {
                m_Rotation.m_Value = new Tools.MathLib.Vector3(0.0f, m_Rotation.m_Value.y - 360.0f, 0.0f);
            }
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            m_Sound.m_Trigger = true;
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
        }
    }
}
