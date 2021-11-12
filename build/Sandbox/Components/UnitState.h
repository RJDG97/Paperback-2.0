// ----- Start of header guard
#ifndef UNITSTATE_H
#define UNITSTATE_H

// how this waypoint USER works :
// ---- Only Objects assign this component follow waypoints
// --------- Everyone Else does not use waypoints
// ---- Waypoint users stop to conduct attacks against other waypoint users
// --------- Once attack is over, recommence movement
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

/* Note:
    // set direction from options
	//int m_dir;

	// if its player, player unit.... else enemy unit (not used)
	bool isPlayer;

	// this defines the player 1-4, in this case, 2 players...
	int m_player;

	// this is to stop in front of an enemy to attack
	bool isAttacking;
*/

// ----- End of header guard
#endif