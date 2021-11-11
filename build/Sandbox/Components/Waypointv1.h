// ----- Start of header guard
#ifndef WAYPOINTV1_H
#define WAYPOINTV1_H

#include "Math/Vector3f.h"

// how this waypoint works :
// ---- destination is bi-directional
// --------- depending on whether its a player unit, they follow different destinations
// ---- pretty much hard-codeed
struct Waypointv1
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Waypointv1"
	};

	// rather than a point, i would rather have a bounding box so that when the unit collide, it assigns a new direction
	paperback::Vector3f m_point;
	paperback::Vector3f m_destination[2]; // [0] is player, [1] is enemy
	paperback::Vector3f m_setspeed[2];
	unsigned int t_to_target[2]; // for ease of use, no need set speed

	// enable speed assigning
	bool b_enableSpeed;
	
	// this is to stop in front of a base
	bool isBase;

	Waypointv1() :
		m_point{ paperback::Vector3f{} },
		b_enableSpeed{ false },
		isBase{false}
	{
		m_destination[0] = m_destination[1] = paperback::Vector3f{};
		m_setspeed[0] = m_setspeed[1] = paperback::Vector3f{};
		t_to_target[0] = t_to_target[1] = 0;
	}

	void Set_d1(float t)
	{
		if (m_point != m_destination[0])
		{
			m_setspeed[0] = (m_destination[0] - m_point) / t;
		}
	}

	void Set_d2(float t)
	{
		if (m_point != m_destination[1])
		{
			m_setspeed[1] = (m_destination[1] - m_point) / t;
		}
	}
};

/* Note:
    // rather than a point, i would rather have a bounding box so that when the unit collide, it assigns a new direction
	// with bounding box, sharp corners (90 degrees) are not recomended
	paperback::Vector3f m_point;

	// as a temporary solution, can also set destination = point when in front of base
	paperback::Vector3f m_destination[2];
	
	// if its for player unit.... else enemy unit (not used)
	bool isPlayer;

	// this is to stop in front of a base (maybe need for functionality???)
	bool isBase;

	// set direction from options
	//int m_dir;
*/

// ----- End of header guard
#endif