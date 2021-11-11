// ----- Start of header guard
#ifndef WAYPOINTUSERV1_H
#define WAYPOINTUSERV1_H

#include "Math/Vector3f.h"

// how this waypoint USER works :
// ---- Only Objects assign this component follow waypoints
// --------- Everyone Else does not use waypoints
// ---- Waypoint users stop to conduct attacks against other waypoint users
// --------- Once attack is over, recommence movement
struct waypointUserv1
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "WaypointUserv1"
	};

	// this defines the player 1-4, in this case, 2 players... 
	int m_player;
	// this is to stop in front of an ally/enemy to attack
	bool isAttacking;
	// destination
	paperback::Vector3f m_destination;
	
	// player should be m_player = 0, enemy should be m_player = 1 (set in sandbox.cpp)
	waypointUserv1() :
		m_player{0},
		isAttacking{false},
		m_destination{ paperback::Vector3f {} }
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