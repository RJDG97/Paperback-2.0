#pragma once

struct timer_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "timer_system"
	};

	void operator()( timer& Timer ) noexcept
	{
		Timer.m_Value -= DeltaTime();
	}
};