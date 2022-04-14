/**********************************************************************************
*\file         Fan.cs
*\brief        Fan.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

        public void PreUpdate(float dt)
        {
            Application.NotifyDone();
        }

        public void Update(float dt)
        {
            if (m_Rotation != null)
            {
                m_Rotation.m_Value += m_Speed;

                if (m_Rotation.m_Value.y >= 360)
                {
                    m_Rotation.m_Value = new Tools.MathLib.Vector3(0.0f, m_Rotation.m_Value.y - 360.0f, 0.0f);
                }
            }

            Application.NotifyDone();
        }
        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {

            Application.NotifyDone();
        }
        public void OnCollisionStay(UInt32 ID)
        {

            Application.NotifyDone();
        }
        public void OnCollisionExit(UInt32 ID)
        {

            Application.NotifyDone();
        }
        public void Reset()
        {
        }
    }
}
