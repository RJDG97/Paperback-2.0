﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class Goal_Cutscene3 : MonoBehaviour
    {
        UInt32 m_ID;
        public static Goal_Cutscene3 getInst()
        {
            return new Goal_Cutscene3();
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
            //if ((ID == Player.GetJumpUnitID() || ID == Player.GetPushUnitID() /*|| collision with blocks*/))
            //{
                Application.ChangeScene("Credits");
            //}
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
