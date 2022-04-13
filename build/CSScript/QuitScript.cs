using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class QuitScript : MonoBehaviour
    {
        UInt32 m_ID;
        public static QuitScript getInst()
        {
            return new QuitScript();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            Debug.Log("Update QuitScript: " + m_ID.ToString());
            if (Input.IsKeyPress(Input.PB_ESCAPE))
                Application.Quit();
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
