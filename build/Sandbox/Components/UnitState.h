#ifndef UNITSTATE_H
#define UNITSTATE_H

struct unitstate
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "unitstate"
	};

	// this is to stop in front of an ally/enemy to attack
	bool isAttacking;
	
	// player should be m_player = 0, enemy should be m_player = 1 (set in sandbox.cpp)
	unitstate() :
		isAttacking{false}
	{}

};

#endif