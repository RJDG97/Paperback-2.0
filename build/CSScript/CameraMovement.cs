using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class CameraMovement : MonoBehaviour
    {
        UInt32 m_ID;
        Transform m_transform;
        PathFollower m_pathfollower;
        public static CameraMovement getInst()
        {
            return new CameraMovement();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_transform = new Transform(m_ID);
            //m_pathfollower = new PathFollower(m_ID);
        }
        public void Update(float dt)
        {
            Tools.MathLib.Vector3 test = new Tools.MathLib.Vector3(0.03f, 0.03f, 0.03f);
            m_transform.m_Position = m_transform.m_Position - test;

            Debug.Log("Position x:");
            Debug.Log(m_transform.m_Position.x.ToString());
            Debug.Log("Position y:");
            Debug.Log(m_transform.m_Position.y.ToString());
            Debug.Log("Position z:");
            Debug.Log(m_transform.m_Position.z.ToString());

            if (Input.IsMouseDown(Input.PB_MOUSE_BUTTON_2))
            {
                if (Input.IsKeyPressDown(Input.PB_W))
                    Camera.MoveForward();
                if (Input.IsKeyPressDown(Input.PB_S))
                    Camera.MoveBackward();
                if (Input.IsKeyPressDown(Input.PB_A))
                    Camera.MoveLeft();
                if (Input.IsKeyPressDown(Input.PB_D))
                    Camera.MoveRight();
                if (Input.IsKeyPressDown(Input.PB_Q))
                    Camera.MoveUp();
                if (Input.IsKeyPressDown(Input.PB_E))
                    Camera.MoveDown();
                if (Input.IsKeyPressDown(Input.PB_UP))
                    Camera.RotateUp();
                if (Input.IsKeyPressDown(Input.PB_DOWN))
                    Camera.RotateDown();
                if (Input.IsKeyPressDown(Input.PB_LEFT))
                    Camera.RotateLeft();
                if (Input.IsKeyPressDown(Input.PB_RIGHT))
                    Camera.RotateRight();
                if (Input.IsKeyPressDown(Input.PB_L))
                {
                    m_pathfollower = new PathFollower(94);
                    m_pathfollower.m_PathID += 1;
                    m_pathfollower.m_PathID = m_pathfollower.m_PathID % 2;
                    m_pathfollower.m_TravelSpeed = 50.0f;
                    m_pathfollower.m_Reversed = !m_pathfollower.m_Reversed;
                }
            }
        }
        public void Destroy()
        {
        }
    }
}
