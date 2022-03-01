using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class Fan : MonoBehaviour
    {
        UInt32 m_ID;
        Rotation m_Rotation;
        Tools.MathLib.Vector3 m_Speed;

        public static Fan getInst()
        {
            return new Fan();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Rotation = new Rotation(m_ID);
            m_Speed = new Tools.MathLib.Vector3(0.0f, 10.0f, 0.0f);
        }
        public void Update(float dt)
        {
            m_Rotation.m_Value += m_Speed;
            if (m_Rotation.m_Value.y >= 360) {
                m_Rotation.m_Value = new Tools.MathLib.Vector3(0.0f, m_Rotation.m_Value.y - 360.0f, 0.0f);
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
    }
}
