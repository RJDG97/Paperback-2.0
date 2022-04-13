/**********************************************************************************
*\file         MonoBehaviour.cs
*\brief        MonoBehaviour.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
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
    public interface MonoBehaviour
    {
        void Start(UInt32 ID);
        void Update(float dt);
        void Destroy();

        void OnCollisionEnter(UInt32 ID);
        void OnCollisionStay(UInt32 ID);
        void OnCollisionExit(UInt32 ID);
    }
}
