/**********************************************************************************
*\file         TimerSystem.h
*\brief        TimerSystem.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct timer_system : paperback::system::pausable_instance
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