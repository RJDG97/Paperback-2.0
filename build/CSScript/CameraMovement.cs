﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class CameraMovement : MonoBehaviour
    {
        public static CameraMovement getInst()
        {
            return new CameraMovement();
        }
        public void Start()
        {
        }
        public void Update(float dt)
        {
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
            }
        }
        public void Destroy()
        {
        }
    }
}