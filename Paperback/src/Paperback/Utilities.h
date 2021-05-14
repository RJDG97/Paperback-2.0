#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

/****************************************************/
/*!			  Uncomment for submissions
/*!			    (Disables all logging)
/****************************************************/
//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF

#include <ctime>
#include <ratio>
#include <chrono>
#include <memory>
#include <utility>

#include "Core/Types.h"

namespace Paperback
{
	class Timer
	{
	public:

		using HRClock = std::chrono::high_resolution_clock;
		using TimePoint = HRClock::time_point;
		using Duration = std::chrono::duration<float>;

		const float fixed_dt;

		Timer(float ts = 1.0f);
		~Timer() = default;

		u32 FPS();
		void Tick();
		void Reset();
		float DeltaTime() const;
		auto Now() -> decltype(HRClock::now());
		float ComputeTime(TimePoint starting_time);

		void TimeScale(float s = 1.0f);
		float TimeScale() const;

	private:

		float elapsed;
		float time_scale;
		Duration delta_time;
		TimePoint start;
	};

	
}

#endif // !UTILITIES_H