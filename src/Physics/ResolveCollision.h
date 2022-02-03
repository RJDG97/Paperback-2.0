#ifndef RESOLVE_COLLISION_H
#define RESOLVE_COLLISION_H

//#include "Components/BoundingBox.h"
//#include "Components/Transform.h"
//#include <limits>
#include <stdlib.h>
#include "Components/RigidForce.h"
#include "Components/Rigidbody.h"
#include "Components/Mass.h"

#define Gap 0.001f
#define BOTH_STATIC 0
#define BOTH_NONSTATIC 1
#define OBJ1_NONSTATIC 2
#define OBJ2_NONSTATIC 3

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

enum direction
{
	x,
	y,
	z,
	none
};


// combined
void Elastic_InElastic_1D(float& v1, float& a1, const float mass1,
						float& v2, float& a2, const float mass2, const float restituition)// 0.f - 1.f
{
	float total_mass = mass1 + mass2;

	// general formula for resolving velocity and acceleration
	float obj1 = (
		(restituition * mass2) * (v2 - v1) + ((mass1 * v1) + (mass2 * v2))
		) / total_mass;
	float obj2 = (
		(restituition * mass1) * (v1 - v2) + ((mass1 * v1) + (mass2 * v2))
		) / total_mass;
	float obj3 = (
		(restituition * mass2) * (a2 - a1) + ((mass1 * a1) + (mass2 * a2))
		) / total_mass;
	float obj4 = (
		(restituition * mass1) * (a1 - a2) + ((mass1 * a1) + (mass2 * a2))
		) / total_mass;

	// updates both objs velocity and acceleration
	v1 = obj1;
	v2 = obj2;
	a1 = obj3;
	a2 = obj4;
}

// pseudo
bool CheapaabbDynamic( boundingbox* Bbox1, rigidforce* rf1, transform& t1, mass* m1,
					   boundingbox* Bbox2, rigidforce* rf2, transform& t2, mass* m2 )
{
	//if ( m1 && m2 && ( m1->m_Mass == 0.f || m2->m_Mass == 0.f ) )
	//	return false;

	paperback::Vector3f vel1{}, vel2{}, acc1{}, acc2{};
	float mass1{}, mass2{};



	// Replacement for below
	if ( m1 && m1->m_Mass != 0.0f )
	{
		vel1 = rf1->m_Momentum / m1->m_Mass;
		acc1 = rf1->m_Forces / m1->m_Mass;
		mass1 = m1->m_Mass;
	}
	if ( m2 && m2->m_Mass != 0.0f )
	{
		vel2 = rf2->m_Momentum / m2->m_Mass;
		acc2 = rf2->m_Forces / m2->m_Mass;
		mass2 = m2->m_Mass;
	}


	//used to decide resolution type
	//0 - both are non-moving, do nothing
	//1 - both are moving, do both
	//2 - only obj1 is moving, do obj1
	//3 - only obj2 is moving, do obj2
	int resolutioncase = (mass1) ? ((mass2) ? 1 : 2) : ((mass2) ? 3 : 0);

	// Abs Resultant Vector
	paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	// Position Difference - Dist between both entities
	paperback::Vector3f ab = (t1.m_Position + t1.m_Offset) - (t2.m_Position + t2.m_Offset);

	//since bounding box values are relative coordinates, need to convert to global coordinates
	paperback::Vector3f box1_min, box1_max, box2_min, box2_max;
	box1_min = t1.m_Position + t1.m_Offset + Bbox1->Min;
	box1_max = t1.m_Position + t1.m_Offset + Bbox1->Max;
	box2_min = t2.m_Position + t2.m_Offset + Bbox2->Min;
	box2_max = t2.m_Position + t2.m_Offset + Bbox2->Max;

	// get pen_depth (+ve), usually ** a super small value
	// small value when fps = 30 ~ 60,  if 1 fps, likely to be a large value
	// Penetration Depth
	paperback::Vector3f pen_depth = (((box1_max - box1_min) + (box2_max - box2_min)) / 2)		// Length between the 2 Colliders
		                            - paperback::Vector3f(abs(ab.x), abs(ab.y), abs(ab.z));				// Distance between the 2 Colliders

	// case 1/3, useless cases is 0.f - currently (+ve)
	// Abs Resultant Vector
	paperback::Vector3f t_resolve = paperback::Vector3f(abs(velab.x), abs(velab.y), abs(velab.z));

	//EDIT
	//alternative use instead of xx/yy/zz but using "face area"
	//concept uses "surface area with highest ratio of contact"
	//makes following assumptions:
	//	1) collision detection happened already, so only need to check surface area ratios
	//	2) objects are of a relatively uniform cube, aka opposing sides have the same area
	//  3) ratio is against smallest face
	//only calculates for 3 axis and based on that isolates the cases
	//main logic: 
	//	1) get area of contact between the two
	//	2) set area over smallest area in comparison to get ratio
	//	3) apply for other 2 axis

	float rel_horizontal_contact, rel_vertical_contact, small_area, area_one, area_two, x_ratio, y_ratio, z_ratio;

	//start with faces along the X axis, uses z and y values for calculation
	rel_horizontal_contact = std::min(box1_max.z, box2_max.z) - std::max(box1_min.z, box2_min.z);
	rel_vertical_contact = std::min(box1_max.y, box2_max.y) - std::max(box1_min.y, box2_min.y);
	area_one = (box1_max.y - box1_min.y) * (box1_max.z - box1_min.z);
	area_two = (box2_max.y - box2_min.y) * (box2_max.z - box2_min.z);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	x_ratio = abs(rel_horizontal_contact * rel_vertical_contact) / small_area;

	//follow up with faces along the y axis, uses x and z values for calculation
	rel_horizontal_contact = std::min(box1_max.z, box2_max.z) - std::max(box1_min.z, box2_min.z);
	rel_vertical_contact = std::min(box1_max.x, box2_max.x) - std::max(box1_min.x, box2_min.x);
	area_one = (box1_max.x - box1_min.x) * (box1_max.z - box1_min.z);
	area_two = (box2_max.x - box2_min.x) * (box2_max.z - box2_min.z);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	y_ratio = abs((rel_horizontal_contact * rel_vertical_contact) / small_area);

	//finally end with faces along the z axis, uses x and y values for calculation
	rel_horizontal_contact = std::min(box1_max.x, box2_max.x) - std::max(box1_min.x, box2_min.x);
	rel_vertical_contact = std::min(box1_max.y, box2_max.y) - std::max(box1_min.y, box2_min.y);
	area_one = (box1_max.x - box1_min.x) * (box1_max.x - box1_min.x);
	area_two = (box2_max.y - box2_min.y) * (box2_max.y - box2_min.y);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	z_ratio = abs(rel_horizontal_contact * rel_vertical_contact) / small_area;




	// determine collision side, smaller ratio = likely side
	direction dir = direction::none;

	// Bounciness Scale -> Restitution = 1.0f for max Bounciness
	float restitution = (rf1->m_Restitution + rf2->m_Restitution) / 2;


	if (x_ratio > y_ratio && x_ratio > z_ratio)
	{
		dir = direction::x;
		// warning!!!! t_resolve , likely pen_depth issue causing snapping problem
		// case 3
		if (ab.x > 0.f)
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				t2.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				t2.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
			}
		}
		// case 1
		else
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				t2.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				t2.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
			}
		}

		//// both objs are movable
		//if (mass1 > 0.f && mass2 > 0.f)
		//{
		Elastic_InElastic_1D(vel1.x, acc1.x, mass1/*m1->m_Mass*/,
			vel2.x, acc2.x, mass2/*m2->m_Mass*/, restitution);
		//}
		//// object 1 has mass, object 2 is immovable
		//else if (mass1 > 0.f)
		//{
		//	vel1.x = acc1.x = 0.f;
		//}
		//// object 2 has mass, object 1 is immovable
		//else if (mass2 > 0.f)
		//{
		//	vel2.x = acc2.x = 0.f;
		//}
		//else
		//{
		//	// in the event both mass are 0.f, do nothing
		//}
		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	}
	else if (y_ratio > x_ratio && y_ratio > z_ratio)
	{
		dir = direction::y;
		// case 3
		if (ab.y > 0.f)
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				//obj1 above, reset grav
				rf1->m_GravityActive = false;
				t1.m_Position.y += (m1 && t_resolve.y > 0.0f) ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
				t2.m_Position.y -= (m2 && t_resolve.y > 0.0f) ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				//obj1 above, reset grav
				rf1->m_GravityActive = false;
				t1.m_Position.y += (m1) ? (pen_depth.y + Gap) : 0.0f;
				rf1->m_Momentum.y = 0.0f;
				rf1->m_Forces.y = 0.0f;
				acc1.y = 0.0f;
				vel1.y = 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				t2.m_Position.y -= (m2) ? (pen_depth.y + Gap) : 0.0f;
				rf2->m_Momentum.y = 0.0f;
				rf2->m_Forces.y = 0.0f;
			}
		}
		// case 1
		else
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				//obj2 above, reset grav
				rf2->m_GravityActive = false;
				t1.m_Position.y -= (m1 && t_resolve.y > 0.0f) ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
				t2.m_Position.y += (m2 && t_resolve.y > 0.0f) ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				t1.m_Position.y -= (m1) ? (pen_depth.y + Gap) : 0.0f;
				rf1->m_Momentum.y = 0.0f;
				rf1->m_Forces.y = 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				//obj2 above, reset grav
				rf2->m_GravityActive = false;
				t2.m_Position.y += (m2) ? (pen_depth.y + Gap) : 0.0f;
				rf1->m_Momentum.y = 0.0f;
				rf1->m_Forces.y = 0.0f;
				acc2.y = 0.0f;
				vel2.y = 0.0f;
			}
		}
		//// both objs are movable
		//if (mass1 > 0.f && mass2 > 0.f)
		//{
		Elastic_InElastic_1D(vel1.y, acc1.y, mass1/*m1->m_Mass*/,
			vel2.y, acc2.y, mass2/*m2->m_Mass*/, restitution);
		//}
		//// object 1 has mass, object 2 is immovable
		//else if (mass1 > 0.f)
		//{
		//	vel1.y = acc1.y = 0.f;
		//}
		//// object 2 has mass, object 1 is immovable
		//else if (mass2 > 0.f)
		//{
		//	vel2.y = acc2.y = 0.f;
		//}
		//else
		//{
		//	// in the event both mass are 0.f, do nothing
		//}

		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	}
	else if (z_ratio > x_ratio && z_ratio > y_ratio)
	{
		dir = direction::z; // Qn: Why got 2 z axis checks?
			   // case 3
		if (ab.z > 0.f)
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				t1.m_Position.z += (m1) ? (pen_depth.z + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				t2.m_Position.z -= (m2) ? (pen_depth.z + Gap) : 0.0f;
			}
		}
		// case 1
		else
		{
			if (resolutioncase == BOTH_NONSTATIC)
			{
				t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ1_NONSTATIC)
			{

				t1.m_Position.z -= (m1) ? (pen_depth.z + Gap) : 0.0f;
			}
			else if (resolutioncase == OBJ2_NONSTATIC)
			{

				t2.m_Position.z += (m2) ? (pen_depth.z + Gap) : 0.0f;
			}
		}

		//// both objs are movable
		//if (mass1 > 0.f && mass2 > 0.f)
		//{
		Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
			vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);
		//}
		//// object 1 has mass, object 2 is immovable
		//else if (mass1 > 0.f)
		//{
		//	vel1.z = acc1.z = 0.f;
		//}
		//// object 2 has mass, object 1 is immovable
		//else if (mass2 > 0.f)
		//{
		//	vel2.z = acc2.z = 0.f;
		//}
		//else
		//{
		//	// in the event both mass are 0.f, do nothing
		//}
		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	}

	// determined side, higher = earlier intersect,
	// resolve penetration depth, aabb style      after that resolve momentum/force
	//if (x_ratio > y_ratio)//(xx > yy)
	//{
	//	if (x_ratio > z_ratio)//(xx > zz)
	//	{
	//		dir = direction::x;
	//		// warning!!!! t_resolve , likely pen_depth issue causing snapping problem
	//		// case 3
	//		if (ab.x > 0.f)
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
	//				t2.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 2)
	//			{
	//				
	//				t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t1.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
	//			}
	//		}
	//		// case 1
	//		else
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
	//				t2.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 2)
	//			{

	//				t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t1.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
	//			}
	//		}

	//		//// both objs are movable
	//		//if (mass1 > 0.f && mass2 > 0.f)
	//		//{
	//			Elastic_InElastic_1D(vel1.x, acc1.x, mass1/*m1->m_Mass*/,
	//				vel2.x, acc2.x, mass2/*m2->m_Mass*/, restitution);
	//		//}
	//		//// object 1 has mass, object 2 is immovable
	//		//else if (mass1 > 0.f)
	//		//{
	//		//	vel1.x = acc1.x = 0.f;
	//		//}
	//		//// object 2 has mass, object 1 is immovable
	//		//else if (mass2 > 0.f)
	//		//{
	//		//	vel2.x = acc2.x = 0.f;
	//		//}
	//		//else
	//		//{
	//		//	// in the event both mass are 0.f, do nothing
	//		//}
	//		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
	//		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	//	}
	//	else if (x_ratio < z_ratio)//(xx < zz)
	//	{
	//		dir = direction::z; // Qn: Why got 2 z axis checks?
	//		// case 3
	//		if (ab.z > 0.f)
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 2)
	//			{

	//				t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//			}
	//		}
	//		// case 1
	//		else
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 2)
	//			{

	//				t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//			}
	//		}

	//		//// both objs are movable
	//		//if (mass1 > 0.f && mass2 > 0.f)
	//		//{
	//			Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
	//				vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);
	//		//}
	//		//// object 1 has mass, object 2 is immovable
	//		//else if (mass1 > 0.f)
	//		//{
	//		//	vel1.z = acc1.z = 0.f;
	//		//}
	//		//// object 2 has mass, object 1 is immovable
	//		//else if (mass2 > 0.f)
	//		//{
	//		//	vel2.z = acc2.z = 0.f;
	//		//}
	//		//else
	//		//{
	//		//	// in the event both mass are 0.f, do nothing
	//		//}
	//		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
	//		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	//	}
	//}
	//else if (x_ratio < y_ratio)//(xx < yy)
	//{
	//	if (y_ratio > z_ratio)//(yy > zz)
	//	{
	//		dir = direction::y;
	//		// case 3
	//		if (ab.y > 0.f)
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.y += (m1 && t_resolve.y > 0.0f) ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
	//				t2.m_Position.y -= (m2 && t_resolve.y > 0.0f) ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;

	//			}
	//			else if (resolutioncase == 2) 
	//			{

	//				t1.m_Position.y += (m1 && t_resolve.y > 0.0f) ? (pen_depth.y + Gap) : 0.0f;
	//				rf1->m_Momentum.y = 0.0f;
	//				rf1->m_Forces.y = 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t2.m_Position.y -= (m2 && t_resolve.y > 0.0f) ? (pen_depth.y + Gap) : 0.0f;
	//				rf2->m_Momentum.y = 0.0f;
	//				rf2->m_Forces.y = 0.0f;
	//			}
	//		}
	//		// case 1
	//		else
	//		{
	//			if (resolutioncase == 1)
	//			{
	//				t1.m_Position.y -= (m1 && t_resolve.y > 0.0f) ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
	//				t2.m_Position.y += (m2 && t_resolve.y > 0.0f) ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 2)
	//			{

	//				t1.m_Position.y -= (m1 && t_resolve.y > 0.0f) ? (pen_depth.y + Gap) : 0.0f;
	//			}
	//			else if (resolutioncase == 3)
	//			{

	//				t1.m_Position.y += (m2 && t_resolve.y > 0.0f) ? (pen_depth.y + Gap) : 0.0f;
	//			}
	//		}
	//		//// both objs are movable
	//		//if (mass1 > 0.f && mass2 > 0.f)
	//		//{
	//			Elastic_InElastic_1D(vel1.y, acc1.y, mass1/*m1->m_Mass*/,
	//				vel2.y, acc2.y, mass2/*m2->m_Mass*/, restitution);
	//		//}
	//		//// object 1 has mass, object 2 is immovable
	//		//else if (mass1 > 0.f)
	//		//{
	//		//	vel1.y = acc1.y = 0.f;
	//		//}
	//		//// object 2 has mass, object 1 is immovable
	//		//else if (mass2 > 0.f)
	//		//{
	//		//	vel2.y = acc2.y = 0.f;
	//		//}
	//		//else
	//		//{
	//		//	// in the event both mass are 0.f, do nothing
	//		//}

	//		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
	//		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	//	}

	//	else if (y_ratio < z_ratio)//(yy < zz)
	//	{
	//		dir = direction::z;
	//		// case 3
	//		//if (ab.z > 0.f)
	//		//{
	//		//	t1.m_Position.z += (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap);
	//		//	t2.m_Position.z -= (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap);
	//		//}
	//		//// case 1
	//		//else
	//		//{
	//		//	t1.m_Position.z -= (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap);
	//		//	t2.m_Position.z += (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap);
	//		//}


	//		if (ab.z > 0.f)
 //           {

 //               if (t_resolve.z > 0.0f)
 //               {
	//				if (resolutioncase == 1)
	//				{
	//					t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//					t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 2)
	//				{

	//					t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 3)
	//				{

	//					t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
 //               }
 //               else
 //               {

	//				if (resolutioncase == 1)
	//				{
	//					t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//					t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 2)
	//				{

	//					t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 3)
	//				{

	//					t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
 //               }
 //           }
 //           // case 1
 //           else
 //           {
	//			if (t_resolve.z > 0.0f)
	//			{
	//				if (resolutioncase == 1)
	//				{
	//					t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//					t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 2)
	//				{

	//					t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 3)
	//				{

	//					t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//			}
	//			else
	//			{

	//				if (resolutioncase == 1)
	//				{
	//					t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//					t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 2)
	//				{

	//					t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//				else if (resolutioncase == 3)
	//				{

	//					t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (pen_depth.z + Gap) : 0.0f;
	//				}
	//			}
 //           }

	//		//// both objs are movable
	//		//if (mass1 > 0.f && mass2 > 0.f)
	//		//{
	//			Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
	//				vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);
	//		//}
	//		//// object 1 has mass, object 2 is immovable
	//		//else if (mass1 > 0.f)
	//		//{
	//		//	vel1.z = acc1.z = 0.f;
	//		//}
	//		//// object 2 has mass, object 1 is immovable
	//		//else if (mass2 > 0.f)
	//		//{
	//		//	vel2.z = acc2.z = 0.f;
	//		//}
	//		//else
	//		//{
	//		//	// in the event both mass are 0.f, do nothing
	//		//}

	//		rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
	//		rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
	//		rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
	//	}
	//}
	return true;
}

//verifies that a slope is set correctly
bool SlopeValid(slope& Slope, int& result)
{

	if (Slope.m_North && !Slope.m_South && !Slope.m_East && !Slope.m_West)
	{

		result = NORTH;
		return true;
	}
	else if (!Slope.m_North && Slope.m_South && !Slope.m_East && !Slope.m_West)
	{

		result = SOUTH;
		return true;
	}
	else if (!Slope.m_North && !Slope.m_South && Slope.m_East && !Slope.m_West)
	{

		result = EAST;
		return true;
	}
	else if (!Slope.m_North && !Slope.m_South && !Slope.m_East && Slope.m_West)
	{

		result = WEST;
		return true;
	}

	return false;
}

bool SlopeaabbDynamic(boundingbox* Bbox1, rigidforce* rf1, transform& t1, mass* m1, slope* Slope1,
	boundingbox* Bbox2, rigidforce* rf2, transform& t2, mass* m2, slope* Slope2)
{
	
	bool slope1_valid, slope2_valid;
	int slope1_dir, slope2_dir;
	slope1_valid = (Slope1) ? SlopeValid(*Slope1, slope1_dir) : false;
	slope2_valid = (Slope2) ? SlopeValid(*Slope2, slope2_dir) : false;

	//if both are not slopes or are slopes, then is invalid comparison
	if (((!slope1_valid && !slope2_valid)
		|| (slope1_valid && slope2_valid))
		|| (!m1 && !m2)
		|| (m1 && m2 && !m1->m_Mass && !m2->m_Mass))
		return false;

	paperback::Vector3f vel1{}, vel2{}, acc1{}, acc2{};
	float mass1{}, mass2{};



	// Replacement for below
	if (m1 && m1->m_Mass != 0.0f)
	{
		vel1 = rf1->m_Momentum / m1->m_Mass;
		acc1 = rf1->m_Forces / m1->m_Mass;
		mass1 = m1->m_Mass;
	}
	if (m2 && m2->m_Mass != 0.0f)
	{
		vel2 = rf2->m_Momentum / m2->m_Mass;
		acc2 = rf2->m_Forces / m2->m_Mass;
		mass2 = m2->m_Mass;
	}

	//only acceptable case is either valid

	//used to decide resolution type
	//0 - both are non-moving, do nothing
	//1 - both are moving, do both
	//2 - only obj1 is moving, do obj1
	//3 - only obj2 is moving, do obj2
	int resolutioncase = (mass1) ? ((mass2) ? 1 : 2) : ((mass2) ? 3 : 0);

	// Abs Resultant Vector
	paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	// Position Difference - Dist between both entities
	paperback::Vector3f ab = (t1.m_Position + t1.m_Offset) - (t2.m_Position + t2.m_Offset);

	//since bounding box values are relative coordinates, need to convert to global coordinates
	paperback::Vector3f box1_min, box1_max, box2_min, box2_max;
	box1_min = t1.m_Position + t1.m_Offset + Bbox1->Min;
	box1_max = t1.m_Position + t1.m_Offset + Bbox1->Max;
	box2_min = t2.m_Position + t2.m_Offset + Bbox2->Min;
	box2_max = t2.m_Position + t2.m_Offset + Bbox2->Max;

	// get pen_depth (+ve), usually ** a super small value
	// small value when fps = 30 ~ 60,  if 1 fps, likely to be a large value
	// Penetration Depth
	paperback::Vector3f pen_depth = (((box1_max - box1_min) + (box2_max - box2_min)) / 2)		// Length between the 2 Colliders
		- paperback::Vector3f(abs(ab.x), abs(ab.y), abs(ab.z));				// Distance between the 2 Colliders

// case 1/3, useless cases is 0.f - currently (+ve)
// Abs Resultant Vector
	paperback::Vector3f t_resolve = paperback::Vector3f(abs(velab.x), abs(velab.y), abs(velab.z));

	//EDIT
	//alternative use instead of xx/yy/zz but using "face area"
	//concept uses "surface area with highest ratio of contact"
	//makes following assumptions:
	//	1) collision detection happened already, so only need to check surface area ratios
	//	2) objects are of a relatively uniform cube, aka opposing sides have the same area
	//  3) ratio is against smallest face
	//only calculates for 3 axis and based on that isolates the cases
	//main logic: 
	//	1) get area of contact between the two
	//	2) set area over smallest area in comparison to get ratio
	//	3) apply for other 2 axis

	float rel_horizontal_contact, rel_vertical_contact, small_area, area_one, area_two, x_ratio, y_ratio, z_ratio;

	//start with faces along the X axis, uses z and y values for calculation
	rel_horizontal_contact = std::min(box1_max.z, box2_max.z) - std::max(box1_min.z, box2_min.z);
	rel_vertical_contact = std::min(box1_max.y, box2_max.y) - std::max(box1_min.y, box2_min.y);
	area_one = (box1_max.y - box1_min.y) * (box1_max.z - box1_min.z);
	area_two = (box2_max.y - box2_min.y) * (box2_max.z - box2_min.z);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	x_ratio = abs(rel_horizontal_contact * rel_vertical_contact) / small_area;
	x_ratio = (x_ratio > 1.0f) ? 1.0f : x_ratio;

	//follow up with faces along the y axis, uses x and z values for calculation
	rel_horizontal_contact = std::min(box1_max.z, box2_max.z) - std::max(box1_min.z, box2_min.z);
	rel_vertical_contact = std::min(box1_max.x, box2_max.x) - std::max(box1_min.x, box2_min.x);
	area_one = (box1_max.x - box1_min.x) * (box1_max.z - box1_min.z);
	area_two = (box2_max.x - box2_min.x) * (box2_max.z - box2_min.z);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	y_ratio = abs((rel_horizontal_contact * rel_vertical_contact) / small_area);
	y_ratio = (y_ratio > 1.0f) ? 1.0f : y_ratio;

	//finally end with faces along the z axis, uses x and y values for calculation
	rel_horizontal_contact = std::min(box1_max.x, box2_max.x) - std::max(box1_min.x, box2_min.x);
	rel_vertical_contact = std::min(box1_max.y, box2_max.y) - std::max(box1_min.y, box2_min.y);
	area_one = (box1_max.x - box1_min.x) * (box1_max.x - box1_min.x);
	area_two = (box2_max.y - box2_min.y) * (box2_max.y - box2_min.y);
	area_one = abs(area_one);
	area_two = abs(area_two);
	small_area = (area_one < area_two) ? area_one : area_two;

	z_ratio = abs(rel_horizontal_contact * rel_vertical_contact) / small_area;
	z_ratio = (z_ratio > 1.0f) ? 1.0f : z_ratio;



	// determine collision side, smaller ratio = likely side
	direction dir = direction::none;

	// Bounciness Scale -> Restitution = 1.0f for max Bounciness
	float restitution = (rf1->m_Restitution + rf2->m_Restitution) / 2;

	//two top cases
	//1) north/south
	//		- if x ratio largest then regular
	//		- if y/z ratio larger, then run line check algo on centre of base of non-slope 
	//2) east/west
	//		- if z ratio largest then regular
	//		- if x/y ratio larger, then run line check algo on centre of base of non-slope


	if ((slope1_valid && (slope1_dir == NORTH || slope1_dir == SOUTH))
		|| (slope2_valid && (slope2_dir == NORTH || slope2_dir == SOUTH)))
	{

		//north/south case
		

		//determine which is slope
		paperback::Vector3f slope_max, slope_min, obj_max, obj_min, obj_dim, obj_base_center, slope_vec{}, slope_point{};

		//at this point confirmed to be only 1, so can use single bool
		slope_max = (slope1_valid) ? box1_max : box2_max;
		slope_min = (slope1_valid) ? box1_min : box2_min;

		obj_max = (slope1_valid) ? box2_max : box1_max;
		obj_min = (slope1_valid) ? box2_min : box1_min;

		obj_dim = (obj_max - obj_min);

		obj_base_center = obj_max - (0.5f * obj_dim);
		obj_base_center.y -= 0.5f * obj_dim.y;

		//on z-y plane, treat z as X and y as Y
		//need to adjust depending on which slope case

		if (slope1_dir == NORTH || slope2_dir == NORTH)
		{

			slope_vec = slope_max - slope_min;
			slope_point = slope_min;
		}
		else if (slope1_dir == SOUTH || slope2_dir == SOUTH)
		{

			//direction flipped, need to adjust
			//can skip x since not used
			slope_vec.y = slope_max.y - slope_min.y;
			slope_vec.z = slope_min.z - slope_max.z;
			slope_point.y = slope_min.y;
			slope_point.z = slope_max.z;
		}

		//now find y value for x value
		//Y_final = point_on_slope.y + slope_vec.y * (obj_base.x - point_on_slope.x) / slope_vec.x
		float final_y_base = slope_point.y + slope_vec.y * (obj_base_center.z - slope_point.z) / slope_vec.z;

		if (x_ratio > y_ratio && x_ratio > z_ratio && final_y_base > obj_base_center.y)
		{
			dir = direction::x;

			// case 3
			if (ab.x > 0.f)
			{
				/*if (resolutioncase == BOTH_NONSTATIC)
				{
					t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
					t2.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				}
				else */
				if (resolutioncase == OBJ1_NONSTATIC && slope2_valid)
				{

					t1.m_Position.x += (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
				}
				else if (resolutioncase == OBJ2_NONSTATIC && slope1_valid)
				{

					t2.m_Position.x -= (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
				}
			}
			// case 1
			else
			{
				/*if (resolutioncase == BOTH_NONSTATIC)
				{
					t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
					t2.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				}
				else*/ 
				if (resolutioncase == OBJ1_NONSTATIC && slope2_valid)
				{

					t1.m_Position.x -= (m1 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
				}
				else if (resolutioncase == OBJ2_NONSTATIC && slope1_valid)
				{

					t2.m_Position.x += (m2 && t_resolve.x > 0.0f) ? (pen_depth.x + Gap) : 0.0f;
				}
			}

			Elastic_InElastic_1D(vel1.x, acc1.x, mass1/*m1->m_Mass*/,
				vel2.x, acc2.x, mass2/*m2->m_Mass*/, restitution);

			rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}
		else
		{
			//assume if fullly overlapped then already inside 
			//do height algo
			//whereever on x axis doesn't matter
			//1) project centre of nonstatic obj's base onto z-y plane
			//2) find corresponding y-value on line segment of slope for said x-value
			//3) convert y-value at base to what value of center of non-static should be


			//error clear case, only perform if center of base is within line segment influence
			if (obj_base_center.z < slope_min.z || obj_base_center.z > slope_max.z)
				return true;


			//set value of object to height
			if (slope1_valid)
			{

				if (final_y_base > obj_base_center.y)
				{

					t2.m_Position.y = (m1) ? (final_y_base + 0.5f * obj_dim.y + Gap) : 0.0f;
					rf2->m_Momentum.y = 0.0f;
					rf2->m_Forces.y = 0.0f;
					rf2->m_GravityActive = false;
				}
			}
			else
			{

				if (final_y_base > obj_base_center.y)
				{

					t1.m_Position.y = (m2) ? (final_y_base + 0.5f * obj_dim.y + Gap) : 0.0f;
					rf1->m_Momentum.y = 0.0f;
					rf1->m_Forces.y = 0.0f;
					rf1->m_GravityActive = false;
				}
			}
		}
	}
	else if ((slope1_valid && (slope1_dir == EAST || slope1_dir == WEST))
		|| (slope2_valid && (slope2_dir == EAST || slope2_dir == WEST)))
	{

		//east/west case
		//determine which is slope
		paperback::Vector3f slope_max, slope_min, obj_max, obj_min, obj_dim, obj_base_center, slope_vec{}, slope_point{};

		//at this point confirmed to be only 1, so can use single bool
		slope_max = (slope1_valid) ? box1_max : box2_max;
		slope_min = (slope1_valid) ? box1_min : box2_min;

		obj_max = (slope1_valid) ? box2_max : box1_max;
		obj_min = (slope1_valid) ? box2_min : box1_min;

		obj_dim = (obj_max - obj_min);

		obj_base_center = obj_max - (0.5f * obj_dim);
		obj_base_center.y -= 0.5f * obj_dim.y;

		//on z-y plane, treat z as X and y as Y
		//need to adjust depending on which slope case

		if (slope1_dir == WEST || slope2_dir == WEST)
		{

			slope_vec = slope_max - slope_min;
			slope_point = slope_min;
		}
		else if (slope1_dir == EAST || slope2_dir == EAST)
		{

			//direction flipped, need to adjust
			//can skip z since not used
			slope_vec.y = slope_max.y - slope_min.y;
			slope_vec.x = slope_min.x - slope_max.x;
			slope_point.y = slope_min.y;
			slope_point.x = slope_max.x;
		}

		//now find y value for x value
		//Y_final = point_on_slope.y + slope_vec.y * (obj_base.x - point_on_slope.x) / slope_vec.x
		float final_y_base = slope_point.y + slope_vec.y * (obj_base_center.x - slope_point.x) / slope_vec.x;

		if (z_ratio > x_ratio && z_ratio > y_ratio && final_y_base > obj_base_center.y)
		{
			dir = direction::z;

				   // case 3
			if (ab.z > 0.f)
			{
				/*if (resolutioncase == BOTH_NONSTATIC)
				{
					t1.m_Position.z += (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
					t2.m_Position.z -= (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				}
				else */
				if (resolutioncase == OBJ1_NONSTATIC && slope2_valid)
				{

					t1.m_Position.z += (m1) ? (pen_depth.z + Gap) : 0.0f;
				}
				else if (resolutioncase == OBJ2_NONSTATIC && slope1_valid)
				{

					t2.m_Position.z -= (m2) ? (pen_depth.z + Gap) : 0.0f;
				}
			}
			// case 1
			else
			{
				/*if (resolutioncase == BOTH_NONSTATIC)
				{
					t1.m_Position.z -= (m1 && t_resolve.z > 0.0f) ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
					t2.m_Position.z += (m2 && t_resolve.z > 0.0f) ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				}
				else*/ 
				if (resolutioncase == OBJ1_NONSTATIC && slope2_valid)
				{

					t1.m_Position.z -= (m1) ? (pen_depth.z + Gap) : 0.0f;
				}
				else if (resolutioncase == OBJ2_NONSTATIC && slope1_valid)
				{

					t2.m_Position.z += (m2) ? (pen_depth.z + Gap) : 0.0f;
				}
			}

			Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
				vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);

			rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}
		else
		{

			//do height algo//error clear case, only perform if center of base is within line segment influence
			if (obj_base_center.x < slope_min.x || obj_base_center.x > slope_max.x)
				return true;


			//set value of object to height
			if (slope1_valid)
			{

				if (final_y_base > obj_base_center.y)
				{

					t2.m_Position.y = (m1) ? (final_y_base + 0.5f * obj_dim.y + Gap) : 0.0f;
					rf2->m_Momentum.y = 0.0f;
					rf2->m_Forces.y = 0.0f;
					rf2->m_GravityActive = false;
				}
			}
			else
			{

				if (final_y_base > obj_base_center.y)
				{

					t1.m_Position.y = (m2) ? (final_y_base + 0.5f * obj_dim.y + Gap) : 0.0f;
					rf1->m_Momentum.y = 0.0f;
					rf1->m_Forces.y = 0.0f;
					rf1->m_GravityActive = false;
				}
			}
		}
	}
	

	return true;
}

bool AABBDynamic(boundingbox* Bbox1, rigidforce* rf1, transform& t1, mass* m1, slope* Slope1,
	boundingbox* Bbox2, rigidforce* rf2, transform& t2, mass* m2, slope* Slope2)
{

	if (!SlopeaabbDynamic(Bbox1, rf1, t1, m1, Slope1, Bbox2, rf2, t2, m2, Slope2))
		return CheapaabbDynamic(Bbox1, rf1, t1, m1, Bbox2, rf2, t2, m2);

	return true;
}

bool CheapaabbBounce(
	boundingbox* Bbox1,
	rigidforce* rf1,
	transform& t1,
	mass* m1,
	boundingbox* Bbox2,
	rigidbody* rb2,
	transform& t2)

	//	paperback::Vector3f& Bbox1min, paperback::Vector3f& Bbox1max,
	//	paperback::Vector3f& mom1, paperback::Vector3f& f1, 
	//	paperback::Vector3f& t1, float mass1,
	//	paperback::Vector3f& Bbox2min, paperback::Vector3f& Bbox2max,
	//	paperback::Vector3f& mom2, paperback::Vector3f& f2, 
	//	paperback::Vector3f& t2, float mass2)
{
	if (m1->m_Mass == 0.f)
		return false;

	paperback::Vector3f vel1 = rf1->m_Momentum / m1->m_Mass;
	paperback::Vector3f vel2 = rb2->m_Velocity;
	paperback::Vector3f acc1 = rf1->m_Forces / m1->m_Mass;
	paperback::Vector3f acc2 = rb2->m_Accel;

	paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	paperback::Vector3f ab = t1.m_Position - t2.m_Position;
	// lol
	// get pen_depth (+ve), shoulld be a super small value
	paperback::Vector3f pen_depth = ((Bbox1->Max - Bbox1->Min) + (Bbox2->Max - Bbox2->Min))
		- paperback::Vector3f(abs(ab.x), abs(ab.y), abs(ab.z));

	// case 1/3, useless cases is 0.f - currently (+ve)
	paperback::Vector3f t_resolve =
		paperback::Vector3f(abs(velab.x), abs(velab.y), abs(velab.z));

	// determine collision side, smaller ratio = likely side
	direction dir = direction::none;
	float xx = 0.f, yy = 0.f, zz = 0.f; // default value

	// cull insignificant values
	// case 2/4
	if ((velab.x > 0.f && ab.x < 0.f) || (velab.x < 0.f && ab.x > 0.f))
		xx = pen_depth.x / t_resolve.x;
	if ((velab.y > 0.f && ab.y < 0.f) || (velab.y < 0.f && ab.y > 0.f))
		yy = pen_depth.y / t_resolve.y;
	if ((velab.z > 0.f && ab.z < 0.f) || (velab.z < 0.f && ab.z > 0.f))
		zz = pen_depth.z / t_resolve.z;

	// determined side, higher = earlier intersect,
	// resolve penetration depth, aabb style      after that resolve momentum/force
	if (xx > yy)
	{
		if (xx > zz)
		{
			dir = direction::x;
			vel1.x = -vel1.x;
			acc1.x = -acc1.x;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
		else
		{
			dir = direction::z;
			vel1.z = -vel1.z;
			acc1.z = -acc1.z;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
	}
	else
	{
		if (yy > zz)
		{
			dir = direction::y;
			vel1.y = -vel1.y;
			acc1.y = -acc1.y;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}

		else
		{
			dir = direction::z;
			vel1.z = -vel1.z;
			acc1.z = -acc1.z;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
	}
	return true;
}

bool CheapaabbStatic(
	boundingbox* Bbox1,
	rigidforce* rf1,
	transform& t1,
	mass* m1,
	boundingbox* Bbox2,
	rigidbody* rb2,
	transform& t2)

	//	paperback::Vector3f& Bbox1min, paperback::Vector3f& Bbox1max,
	//	paperback::Vector3f& mom1, paperback::Vector3f& f1, 
	//	paperback::Vector3f& t1, float mass1,
	//	paperback::Vector3f& Bbox2min, paperback::Vector3f& Bbox2max,
	//	paperback::Vector3f& mom2, paperback::Vector3f& f2, 
	//	paperback::Vector3f& t2, float mass2)
{
	if (m1->m_Mass == 0.f)
		return false;

	paperback::Vector3f vel1 = rf1->m_Momentum / m1->m_Mass;
	paperback::Vector3f vel2 = rb2->m_Velocity;
	paperback::Vector3f acc1 = rf1->m_Forces / m1->m_Mass;
	paperback::Vector3f acc2 = rb2->m_Accel;

	paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	paperback::Vector3f ab = t1.m_Position - t2.m_Position;
	// lol
	// get pen_depth (+ve), shoulld be a super small value
	paperback::Vector3f pen_depth = ((Bbox1->Max - Bbox1->Min) + (Bbox2->Max - Bbox2->Min))
		- paperback::Vector3f(abs(ab.x), abs(ab.y), abs(ab.z));

	// case 1/3, useless cases is 0.f - currently (+ve)
	paperback::Vector3f t_resolve =
		paperback::Vector3f(abs(velab.x), abs(velab.y), abs(velab.z));

	// determine collision side, smaller ratio = likely side
	direction dir = direction::none;
	float xx = 0.f, yy = 0.f, zz = 0.f; // default value

	// cull insignificant values
	// case 2/4
	if ((velab.x > 0.f && ab.x < 0.f) || (velab.x < 0.f && ab.x > 0.f))
		xx = pen_depth.x / t_resolve.x;
	if ((velab.y > 0.f && ab.y < 0.f) || (velab.y < 0.f && ab.y > 0.f))
		yy = pen_depth.y / t_resolve.y;
	if ((velab.z > 0.f && ab.z < 0.f) || (velab.z < 0.f && ab.z > 0.f))
		zz = pen_depth.z / t_resolve.z;

	// determined side, higher = earlier intersect,
	// resolve penetration depth, aabb style      after that resolve momentum/force
	if (xx > yy)
	{
		if (xx > zz)
		{
			dir = direction::x;
			vel1.x = 0.f;
			acc1.x = 0.f;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
		else
		{
			dir = direction::z;
			vel1.z = 0.f;
			acc1.z = 0.f;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
	}
	else
	{
		if (yy > zz)
		{
			dir = direction::y;
			vel1.y = 0.f;
			acc1.y = 0.f;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}

		else
		{
			dir = direction::z;
			vel1.z = 0.f;
			acc1.z = 0.f;

			rf1->m_Momentum = vel1 * m1->m_Mass;
			rf1->m_Forces = acc1 * m1->m_Mass;
		}
	}
	return true;
}

// veresion 1
bool CheapsphereDynamic(
	paperback::Vector3f& mom1, paperback::Vector3f& f1,
	paperback::Vector3f& t1, paperback::Vector3f& r1, float mass1,
	paperback::Vector3f& mom2, paperback::Vector3f& f2,
	paperback::Vector3f& t2, paperback::Vector3f& r2, float mass2)
{
	if (mass1 == 0.f || mass2 == 0.f)
		return false;

	paperback::Vector3f vel1 = mom1 / mass1;
	paperback::Vector3f vel2 = mom2 / mass2;
	paperback::Vector3f acc1 = f1 / mass1;
	paperback::Vector3f acc2 = f2 / mass2;

	//paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	// x.norm
	paperback::Vector3f ab_norm = (t1 - t2).Normalized();

	float x1 = ab_norm.Dot(vel1);
	paperback::Vector3f v1x = ab_norm * x1;
	paperback::Vector3f v1y = vel1 - v1x;
	float a1 = ab_norm.Dot(acc1);
	paperback::Vector3f a1x = ab_norm * a1;
	paperback::Vector3f a1y = acc1 - a1x;

	// opposite x.norm
	paperback::Vector3f opp_ab_norm = ab_norm * -1.f;

	float x2 = opp_ab_norm.Dot(vel2);
	paperback::Vector3f v2x = opp_ab_norm * x2;
	paperback::Vector3f v2y = vel2 - v2x;
	float a2 = opp_ab_norm.Dot(acc2);
	paperback::Vector3f a2x = opp_ab_norm * a2;
	paperback::Vector3f a2y = acc2 - a2x;

	paperback::Vector3f vela =
		v1x * (mass1 - mass2) / (mass1 + mass2)
		+ v2x * (2.f * mass2) / (mass1 + mass2)
		+ v1y;
	paperback::Vector3f velb =
		v1x * (2.f * mass1) / (mass1 + mass2)
		+ v2x * (mass2 - mass1) / (mass1 + mass2)
		+ v2y;

	paperback::Vector3f acca =
		a1x * (mass1 - mass2) / (mass1 + mass2)
		+ a2x * (2.f * mass2) / (mass1 + mass2)
		+ a1y;
	paperback::Vector3f accb =
		a1x * (2.f * mass1) / (mass1 + mass2)
		+ a2x * (mass2 - mass1) / (mass1 + mass2)
		+ a2y;

	mom1 = vela * mass1;
	mom2 = velb * mass2;
	f1 = acca * mass1;
	f2 = accb * mass2;

	return true;
}

//// version 2
//bool ExpAnglesphereDynamic(
//	paperback::Vector3f& mom1, paperback::Vector3f& f1,
//	paperback::Vector3f& t1, paperback::Vector3f& r1, float mass1,
//	paperback::Vector3f& mom2, paperback::Vector3f& f2,
//	paperback::Vector3f& t2, paperback::Vector3f& r2, float mass2)
//{
//	if (mass1 == 0.f || mass2 == 0.f)
//		return false;
//
//	paperback::Vector3f vel1 = mom1 / mass1;
//	paperback::Vector3f vel2 = mom2 / mass2;
//	paperback::Vector3f acc1 = f1 / mass1;
//	paperback::Vector3f acc2 = f2 / mass2;
//
//	// x.norm
//	paperback::Vector3f ab = t1 - t2;
//	paperback::Vector3f ba = t2 - t1;
//
//	// angle
//	float xyz1 = acos((ab.x * vel1.x + ab.y * vel1.y + ab.z * vel1.z)
//		/ sqrtf((ab.x + ab.y + ab.z) * (vel1.x + vel1.y + vel1.z)));
//	float xyz2 = acos((ba.x * vel2.x + ba.y * vel2.y + ba.z * vel2.z)
//		/ sqrtf((ba.x + ba.y + ba.z) * (vel2.x + vel2.y + vel2.z)));
//
//	// center
//	paperback::Vector3f vc1 = vel1.Normalized() * cos(xyz1);
//	paperback::Vector3f vc2 = vel2.Normalized() * cos(xyz2);
//
//	// decompose
//	float vx1 = vc1.x * sinf() * cosf();
//	float vy1 = vc1 * sinf() * sinf();
//	float vz1 = vc1 * cosf();
//
//	// v norm
//	paperback::Vector3f vn1 = vel1 - vc1;
//	paperback::Vector3f vn2 = vel2 - vc2;
//
//	// swap force vector
//
//	// calculate new velocity / acc
//	
//
//	mom1 = vela * mass1;
//	mom2 = velb * mass2;
//	f1 = acca * mass1;
//	f2 = accb * mass2;
//
//	return true;
//}


#endif
