/**********************************************************************************
*\file         QuitScript.cs
*\brief        QuitScript.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
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
