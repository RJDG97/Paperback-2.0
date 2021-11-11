#ifndef RESOLVE_COLLISION_H
#define RESOLVE_COLLISION_H

//#include "Components/BoundingBox.h"
//#include "Components/Transform.h"
//#include <limits>
#include <stdlib.h>
#include "Components/RigidForce.h"

// later throw in GGGKKK

enum direction
{
	x,
	y,
	z,
	none
};

// cheap 1D
void Cheap_Elastic_collision_1D(float& v1, float& a1, const float mass1,
	float& v2, float& a2, const float mass2)
{
	if (mass1 != mass2)
	{
		float total_mass = mass1 + mass2;
		float var1 = (mass1 - mass2) / total_mass,
			var2 = (2 * mass2) / total_mass,
			var3 = (2 * mass1) / total_mass,
			var4 = (mass2 - mass1) / total_mass;

		float obj1 = (var1 * v1)
			+ (var2 * v2);
		float obj2 = (var3 * v1)
			+ (var4 * v2);
		float obj3 = (var1 * a1)
			+ (var2 * a2);
		float obj4 = (var3 * a1)
			+ (var4 * a2);

		v1 = obj1;
		v2 = obj2;
		a1 = obj3;
		a2 = obj4;
	}
	else // Swap
	{
		paperback::MathUtils::Swap<float>(v1, v2);
		paperback::MathUtils::Swap<float>(a1, a2);
	}
}

// pseudo
bool CheapaabbDynamic(
	boundingbox* Bbox1,
	rigidforce* rf1,
	transform& t1,
	boundingbox* Bbox2,
	rigidforce* rf2,
	transform& t2)

//	paperback::Vector3f& Bbox1min, paperback::Vector3f& Bbox1max,
//	paperback::Vector3f& mom1, paperback::Vector3f& f1, 
//	paperback::Vector3f& t1, float mass1,
//	paperback::Vector3f& Bbox2min, paperback::Vector3f& Bbox2max,
//	paperback::Vector3f& mom2, paperback::Vector3f& f2, 
//	paperback::Vector3f& t2, float mass2)
{
	if (rf1->m_Mass == 0.f || rf2->m_Mass == 0.f)
		return false;

	paperback::Vector3f vel1 = rf1->m_Momentum / rf1->m_Mass;
	paperback::Vector3f vel2 = rf2->m_Momentum / rf2->m_Mass;
	paperback::Vector3f acc1 = rf1->m_Forces / rf1->m_Mass;
	paperback::Vector3f acc2 = rf2->m_Forces / rf2->m_Mass;

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
			// warning!!!! t_resolve , likely pen_depth issue causing snapping problem
			//// case 3
			//if (ab.x > 0.f)
			//{
			//	t1.x += (abs(vel1.x) / t_resolve.x * pen_depth.x);
			//	t2.x -= (abs(vel2.x) / t_resolve.x * pen_depth.x);
			//}
			//// case 1
			//else
			//{
			//	t1.x -= (abs(vel1.x) / t_resolve.x * pen_depth.x);
			//	t2.x += (abs(vel2.x) / t_resolve.x * pen_depth.x);
			//}
			// temporary holder before refining
			Cheap_Elastic_collision_1D(vel1.x, acc1.x, rf1->m_Mass,
				vel2.x, acc2.x, rf2->m_Mass);
			rf1->m_Momentum = vel1 * rf1->m_Mass;
			rf2->m_Momentum = vel2 * rf2->m_Mass;
			rf1->m_Forces = acc1 * rf1->m_Mass;
			rf2->m_Forces = acc2 * rf2->m_Mass;
		}
		else
		{
			dir = direction::z;
			//// case 3
			//if (ab.z > 0.f)
			//{
			//	t1.z += (abs(vel1.z) / t_resolve.z * pen_depth.z);
			//	t2.z -= (abs(vel2.z) / t_resolve.z * pen_depth.z);
			//}
			//// case 1
			//else
			//{
			//	t1.z -= (abs(vel1.z) / t_resolve.z * pen_depth.z);
			//	t2.z += (abs(vel2.z) / t_resolve.z * pen_depth.z);
			//}
			Cheap_Elastic_collision_1D(vel1.z, acc1.z, rf1->m_Mass,
				vel2.z, acc2.z, rf2->m_Mass);
			rf1->m_Momentum = vel1 * rf1->m_Mass;
			rf2->m_Momentum = vel2 * rf2->m_Mass;
			rf1->m_Forces = acc1 * rf1->m_Mass;
			rf2->m_Forces = acc2 * rf2->m_Mass;
		}
	}
	else
	{
		if (yy > zz)
		{
			dir = direction::y;
			//// case 3
			//if (ab.y > 0.f)
			//{
			//	t1.y += (abs(vel1.y) / t_resolve.y * pen_depth.y);
			//	t2.y -= (abs(vel2.y) / t_resolve.y * pen_depth.y);
			//}
			//// case 1
			//else
			//{
			//	t1.y -= (abs(vel1.y) / t_resolve.y * pen_depth.y);
			//	t2.y += (abs(vel2.y) / t_resolve.y * pen_depth.y);
			//}
			Cheap_Elastic_collision_1D(vel1.y, acc1.y, rf1->m_Mass,
				vel2.y, acc2.y, rf2->m_Mass);
			rf1->m_Momentum = vel1 * rf1->m_Mass;
			rf2->m_Momentum = vel2 * rf2->m_Mass;
			rf1->m_Forces = acc1 * rf1->m_Mass;
			rf2->m_Forces = acc2 * rf2->m_Mass;
		}
			
		else
		{
			dir = direction::z;
			//// case 3
			//if (ab.z > 0.f)
			//{
			//	t1.z += (abs(vel1.z) / t_resolve.z * pen_depth.z);
			//	t2.z -= (abs(vel2.z) / t_resolve.z * pen_depth.z);
			//}
			//// case 1
			//else
			//{
			//	t1.z -= (abs(vel1.z) / t_resolve.z * pen_depth.z);
			//	t2.z += (abs(vel2.z) / t_resolve.z * pen_depth.z);
			//}
			Cheap_Elastic_collision_1D(vel1.z, acc1.z, rf1->m_Mass,
				vel2.z, acc2.z, rf2->m_Mass);
			rf1->m_Momentum = vel1 * rf1->m_Mass;
			rf2->m_Momentum = vel2 * rf2->m_Mass;
			rf1->m_Forces = acc1 * rf1->m_Mass;
			rf2->m_Forces = acc2 * rf2->m_Mass;
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
	paperback::Vector3f opp_ab_norm = ab_norm  * -1.f;

	float x2 = opp_ab_norm.Dot(vel2);
	paperback::Vector3f v2x = opp_ab_norm * x2;
	paperback::Vector3f v2y = vel2 - v2x;
	float a2 = opp_ab_norm.Dot(acc2);
	paperback::Vector3f a2x = opp_ab_norm * a2;
	paperback::Vector3f a2y = acc2 - a2x;

	paperback::Vector3f vela = 
		v1x * (mass1-mass2)/(mass1+mass2) 
		+ v2x * (2.f * mass2)/ (mass1 + mass2) 
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
