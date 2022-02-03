using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class GameController : MonoBehaviour
    {
        UInt32 m_ID;
        Camera m_JumpUnitCamera;
        Camera m_PushUnitCamera;

        public static GameController getInst()
        {
            return new GameController();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_JumpUnitCamera = new Camera((UInt32)Player.GetJumpUnitID());
            m_PushUnitCamera = new Camera((UInt32)Player.GetPushUnitID());

            m_JumpUnitCamera.m_Active = true;
            m_PushUnitCamera.m_Active = false;
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
