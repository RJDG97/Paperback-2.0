﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    class SplashScreenCollider : MonoBehaviour
    {
        UInt32 m_ID;
        public static SplashScreenCollider getInst()
        {
            return new SplashScreenCollider();
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
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
                Application.ChangeScene("MainMenu");
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
