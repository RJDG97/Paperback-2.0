﻿/**********************************************************************************
*\file         Goal_Cutscene3.cs
*\brief        Goal_Cutscene3.cs Script
*
*\author	     Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class Goal_Cutscene3 : MonoBehaviour
    {
        UInt32 m_ID;

        BoundingBox m_BoundingBox;
        public static Goal_Cutscene3 getInst()
        {
            return new Goal_Cutscene3();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_BoundingBox = new BoundingBox(m_ID);
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
