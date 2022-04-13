/**********************************************************************************
*\file         Timer.cs
*\brief        Timer.cs Script
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

namespace CSScript.Tools
{
    public class Timer
    {
        float m_Duration;
        float m_RemainingTime;
        bool m_Paused;

        public Timer(float duration)
        {
            m_Duration = duration;
            m_RemainingTime = duration;
        }

        public void ChangeDuration(float duration)
        {
            m_Duration = duration;
        }

        public void TogglePauseTimer()
        {
            m_Paused = !m_Paused;
        }

        public void ResetTimer()
        {
            m_RemainingTime = m_Duration;
        }

        public void UpdateTimer(float dt)
        {
            if (!m_Paused && m_RemainingTime > 0.0f)
                m_RemainingTime -= dt;
        }

        public bool TimerFinished()
        {
            return m_RemainingTime > 0.0f;
        }
    }
}
