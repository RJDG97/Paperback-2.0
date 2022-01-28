using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class UnitBehaviour : MonoBehaviour
    {
        UInt32 m_ID;
        public static UnitBehaviour getInst()
        {
            return new UnitBehaviour();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;
        }
        public void Update(float dt)
        {
        }
        public void Destroy()
        {
        }
    }
}
