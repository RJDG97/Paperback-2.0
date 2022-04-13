/**********************************************************************************
*\file         MovingBillboard.cs
*\brief        MovingBillboard.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
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
    public class MovingBillboard : MonoBehaviour
    {
        UInt32 m_ID;
        Freezable m_Freezable;
        ParticleEmitter m_ParticleEmitter;

        public static MovingBillboard getInst()
        {
            return new MovingBillboard();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_Freezable = new Freezable(ID);
            m_ParticleEmitter = new ParticleEmitter(ID);
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            if (m_Freezable.m_Frozen)
            {
                m_ParticleEmitter.m_Lifetime = 0.5f;
            }

            else
            {
                m_ParticleEmitter.m_Lifetime = 0.0f;
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
        public void Reset()
        {
        }
    }
}
