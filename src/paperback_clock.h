/**********************************************************************************
*\file         paperback_clock.h
*\brief        paperback_clock.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <ctime>
#include <ratio>
#include <chrono>
#include <time.h>
#include <memory>
#include <cstdint>
#include <utility>

namespace tools
{
    class clock final
    {
    public:

        using HRClock = std::chrono::high_resolution_clock;
        using TimePoint = HRClock::time_point;
        using Duration = std::chrono::duration<float>;

        PPB_INLINE
        clock( float TimeScale = 1.0f ) noexcept;

        PPB_INLINE
        ~clock() = default;

        PPB_INLINE
        uint32_t FPS() noexcept;

        PPB_INLINE
        void     Tick() noexcept;

        PPB_INLINE
        float    DeltaTime() const noexcept;

        PPB_INLINE
        auto     Now() noexcept -> decltype( HRClock::now() );

        PPB_INLINE
        float    ComputeTime( TimePoint StartingTime ) noexcept;

        PPB_INLINE
        void     TimeScale( const float s = 1.0f ) noexcept;

        PPB_INLINE
        float    TimeScale() const noexcept;

    private:

        const float   m_FixedDT;
        float         m_TimeElapsed;
        float         m_TimeScale;
        Duration      m_DT;
        TimePoint     m_Start;
    };
}