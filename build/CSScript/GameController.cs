using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class GameController : MonoBehaviour
    {
        UInt32 m_ID;
        Camera m_RedCamera;
        Camera m_BlueCamera;

        public static GameController getInst()
        {
            return new GameController();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_RedCamera = new Camera((UInt32)Player.GetRedRobotID());
            m_BlueCamera = new Camera((UInt32)Player.GetBlueRobotID());

            m_RedCamera.m_Active = true;
            m_BlueCamera.m_Active = false;
        }
        public void Update(float dt)
        {
            if (Input.IsKeyPress(Input.PB_Q))
            {
                Player.TogglePlayers();
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
