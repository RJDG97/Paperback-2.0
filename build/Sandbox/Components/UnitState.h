#ifndef UNITSTATE_H
#define UNITSTATE_H

struct unitstate
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Unitstate"
	};

	// this is to stop in front of an ally/enemy to attack
	bool isAttacking;
	
	// player should be m_player = 0, enemy should be m_player = 1 (set in sandbox.cpp)
	unitstate() :
		isAttacking{false}
	{}

};

namespace RR_UnitState
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<unitstate>(unitstate::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
			.property("Attacking", &unitstate::isAttacking);
	}
}

#endif