#ifndef RESOLVE_COLLISION_H
#define RESOLVE_COLLISION_H

//#include "Components/BoundingBox.h"
//#include "Components/Transform.h"
//#include <limits>
#include <stdlib.h>
#include "Components/RigidForce.h"
#include "Components/Rigidbody.h"
#include "Components/Mass.h"

#define Gap 0.01f

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

	//// Relative Velocity
	//paperback::Vector3f vel1 = rf1->m_Momentum / m1->m_Mass;
	//paperback::Vector3f vel2 = rf2->m_Momentum / m2->m_Mass;

	//// Compute Acceleration
	//paperback::Vector3f acc1 = rf1->m_Forces / m1->m_Mass;
	//paperback::Vector3f acc2 = rf2->m_Forces / m2->m_Mass;





	// Abs Resultant Vector
	paperback::Vector3f velab = vel1 - vel2; // uncorrupt
	// Position Difference - Dist between both entities
	paperback::Vector3f ab = t1.m_Position - t2.m_Position;
	
	// get pen_depth (+ve), usually ** a super small value
	// small value when fps = 30 ~ 60,  if 1 fps, likely to be a large value
	// Penetration Depth
	paperback::Vector3f pen_depth = (((Bbox1->Max - Bbox1->Min) + (Bbox2->Max - Bbox2->Min)) / 2)		// Length between the 2 Colliders
		                            - paperback::Vector3f(abs(ab.x), abs(ab.y), abs(ab.z));				// Distance between the 2 Colliders

	// case 1/3, useless cases is 0.f - currently (+ve)
	// Abs Resultant Vector
	paperback::Vector3f t_resolve = paperback::Vector3f(abs(velab.x), abs(velab.y), abs(velab.z));

	// This Covers The Else Case (Basically skips force addition if both objects are stationary within each other)
	if ( t_resolve.x == 0.0f && t_resolve.y == 0.0f && t_resolve.z == 0.0f )
		return true;

	// determine collision side, smaller ratio = likely side
	direction dir = direction::none;
	float xx = 0.f, yy = 0.f, zz = 0.f; // default value

	// for xx, yy, zz
	// distance (pen_depth) = velocity (velab -> total accumulated velocity) * time
	// therefore, time (xx) = distance / velocity
	// in this case, xx will calculate the **** time taken *** to travel the pen_depth distance
	// the smaller the xx, the higher the distance the object travel BEFORE colliding with the other object

	// so the higher the xx, the shorter the distance the object travelled BEFORE collision
	
	// instead of spamming raycasting to aabb (predicting movement), this simply calculates AFTER the collision

	// case 2/4     determine which side hit and whos first
	if ((velab.x > 0.f && ab.x < 0.f) || (velab.x < 0.f && ab.x > 0.f))
		xx = (pen_depth.x) / t_resolve.x;
	if ((velab.y > 0.f && ab.y < 0.f) || (velab.y < 0.f && ab.y > 0.f))
		yy = (pen_depth.y) / t_resolve.y;
	if ((velab.z > 0.f && ab.z < 0.f) || (velab.z < 0.f && ab.z > 0.f))
		zz = (pen_depth.z) / t_resolve.z;

	// Bounciness Scale -> Restitution = 1.0f for max Bounciness
	float restitution = (rf1->m_Restitution + rf2->m_Restitution) / 2;

	// determined side, higher = earlier intersect,
	// resolve penetration depth, aabb style      after that resolve momentum/force
	if (xx > yy)
	{
		if (xx > zz)
		{
			dir = direction::x;
			// warning!!!! t_resolve , likely pen_depth issue causing snapping problem
			// case 3
			if (ab.x > 0.f)
			{
				t1.m_Position.x += m1 ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				t2.m_Position.x -= m2 ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
			}
			// case 1
			else
			{
				t1.m_Position.x -= m1 ? (abs(vel1.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
				t2.m_Position.x += m2 ? (abs(vel2.x) / t_resolve.x * pen_depth.x + Gap) : 0.0f;
			}
			// temporary holder before refining
			Elastic_InElastic_1D(vel1.x, acc1.x, mass1/*m1->m_Mass*/,
				vel2.x, acc2.x, mass2/*m2->m_Mass*/, restitution);
			rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}
		else
		{
			dir = direction::z; // Qn: Why got 2 z axis checks?
			// case 3
			if (ab.z > 0.f)
			{
				t1.m_Position.z += m1 ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				t2.m_Position.z -= m2 ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
			}
			// case 1
			else
			{
				t1.m_Position.z -= m1 ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
				t2.m_Position.z += m2 ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
			}
			Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
				vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);
			rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}
	}
	else
	{
		if (yy > zz)
		{
			dir = direction::y;
			// case 3
			if (ab.y > 0.f)
			{
				t1.m_Position.y += m1 ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
				t2.m_Position.y -= m2 ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
			}
			// case 1
			else
			{
				t1.m_Position.y -= m1 ? (abs(vel1.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
				t2.m_Position.y += m2 ? (abs(vel2.y) / t_resolve.y * pen_depth.y + Gap) : 0.0f;
			}
			Elastic_InElastic_1D(vel1.y, acc1.y, mass1/*m1->m_Mass*/,
				vel2.y, acc2.y, mass2/*m2->m_Mass*/, restitution);
			rf1->m_Momentum = t1.m_Position.y > t2.m_Position.y ? paperback::Vector3f{} : vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = t2.m_Position.y > t1.m_Position.y ? paperback::Vector3f{} : vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}

		else
		{
			dir = direction::z;
			// case 3
			//if (ab.z > 0.f)
			//{
			//	t1.m_Position.z += (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap);
			//	t2.m_Position.z -= (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap);
			//}
			//// case 1
			//else
			//{
			//	t1.m_Position.z -= (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap);
			//	t2.m_Position.z += (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap);
			//}


			if (ab.z > 0.f)
            {

                if (t_resolve.z > 0.0f)
                {
                    t1.m_Position.z += m1 ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
					t2.m_Position.z -= m2 ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
                }
                else
                {

					t1.m_Position.z += m1 ? pen_depth.z : 0.0f;
                }
            }
            // case 1
            else
            {

                if (t_resolve.z > 0.0f)
                {
                    t1.m_Position.z -= m1 ? (abs(vel1.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
					t2.m_Position.z += m2 ? (abs(vel2.z) / t_resolve.z * pen_depth.z + Gap) : 0.0f;
                }
                else
                {

					t2.m_Position.z += m2 ? pen_depth.z : 0.0f;
                }
            }




			Elastic_InElastic_1D(vel1.z, acc1.z, mass1/*m1->m_Mass*/,
				vel2.z, acc2.z, mass2/*m2->m_Mass*/, restitution);
			rf1->m_Momentum = vel1 * mass1/*m1->m_Mass*/;
			rf2->m_Momentum = vel2 * mass2/*m2->m_Mass*/;
			rf1->m_Forces = acc1 * mass1/*m1->m_Mass*/;
			rf2->m_Forces = acc2 * mass2/*m2->m_Mass*/;
		}
	}
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
