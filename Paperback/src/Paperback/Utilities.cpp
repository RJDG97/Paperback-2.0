#include "PaperbackPCH.h"
#include "Utilities.h"

namespace Paperback
{
	Timer::Timer(float ts) :
		fixed_dt{ 1 / 60.0f },
		time_scale{ ts },
		delta_time{ },
		start{ Now() }
	{
		Reset();
	}

	u32 Timer::FPS()
	{
		return static_cast<u32>(1.0f / delta_time.count());
	}

	void Timer::Tick()
	{
		delta_time = HRClock::now() - start;
		start = HRClock::now();

		elapsed += delta_time.count();
	}

	void Timer::Reset()
	{
		start = HRClock::now();
	}

	float Timer::DeltaTime() const
	{
		return delta_time.count();
	}

	auto Timer::Now() -> decltype(HRClock::now())
	{
		return HRClock::now();
	}

	float Timer::ComputeTime(TimePoint starting_time)
	{
		return static_cast<Duration>(HRClock::now() - starting_time).count();
	}

	void Timer::TimeScale(float s)
	{
		time_scale = s;
	}

	float Timer::TimeScale() const
	{
		return time_scale;
	}
}