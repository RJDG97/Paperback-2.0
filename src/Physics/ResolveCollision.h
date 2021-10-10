#ifndef RESOLVE_COLLISION_H
#define RESOLVE_COLLISION_H

//#include "Components/BoundingBox.h"
//#include "Components/Sphere.h"
#include "Components/RigidForce.h"

enum direction
{
	x,
	y,
	z
};

// could include inv mass if want for computation to be visual
void Elastic_collision_1D(paperback::Vector3f& Bbox1, const float mass1,
	paperback::Vector3f& Bbox2, const float mass2, paperback::Vector3f dir)
{
	if (mass1 != mass2)
	{
		float total_mass = mass1 + mass2;
		paperback::Vector3f obj1 = ((mass1 - mass2) * (Bbox1 / total_mass))
			+ ((2 * mass2) * (Bbox2 / total_mass));
		paperback::Vector3f obj2 = ((2 * mass1) * (Bbox1 / total_mass))
			+ ((mass2 - mass1) * (Bbox2 / total_mass));
		Bbox1 = obj1;
		Bbox2 = obj2;
	}
	else // Swap
	{
		paperback::MathUtils::Swap<paperback::Vector3f>(Bbox1, Bbox2);
	}
}

//// could include inv mass if want for computation to be visual
//void Elastic_collision_2D(paperback::Vector3f& Bbox1, const float mass1,
//	paperback::Vector3f& Bbox2, const float mass2)
//{
//	float total_mass = mass1 + mass2;
//	paperback::Vector3f obj1 = ((mass1 - mass2) * (Bbox1 / total_mass))
//		+ ((2 * mass2) * (Bbox2 / total_mass));
//	paperback::Vector3f obj2 = ((2 * mass1) * (Bbox1 / total_mass))
//		+ ((mass2 - mass1) * (Bbox2 / total_mass));
//	Bbox1 = obj1;
//	Bbox2 = obj2;
//
//}

void AABB_AABB_Response(rigidforce* Bbox1, rigidforce* Bbox2, paperback::Vector3f dir)
{
	// points towards obj1
	dir;
	paperback::Vector3f dir1 = Bbox1->m_Momentum / Bbox1->m_Mass;
	paperback::Vector3f dir1norm = dir1.Normalized();
	paperback::Vector3f dir2 = Bbox2->m_Momentum / Bbox2->m_Mass;
	paperback::Vector3f dir2norm = dir2.Normalized();
	Elastic_collision_1D(Bbox1->m_Forces, Bbox1->m_Mass, Bbox2->m_Forces, Bbox2->m_Mass, dir);
	Elastic_collision_1D(Bbox1->m_Momentum, Bbox1->m_Mass, Bbox2->m_Momentum, Bbox2->m_Mass, dir);
}



//void Sphere_Sphere_Response(rigidforce& Bbox1, rigidforce& Bbox2)
//{
//	Elastic_collision_1D(Bbox1.m_Forces, Bbox1.m_Mass, Bbox2.m_Forces, Bbox2.m_Mass);
//	Elastic_collision_1D(Bbox1.m_Momentum, Bbox1.m_Mass, Bbox2.m_Momentum, Bbox2.m_Mass);
//}

//// -- Collision flags [hotspots]
//// -- X-AXIS
//#define	COLLISION_LEFT			0x00000001		// -- 000001
//#define COLLISION_LEFT_RESET	0xFFFFFFFE		// -- 111110
//#define	COLLISION_RIGHT			0x00000002		// -- 000010
//#define COLLISION_RIGHT_RESET	0xFFFFFFFD		// -- 111101
//
//// -- Y-AXIS
//#define COLLISION_FORWARD			0x00000004		// -- 000100
//#define COLLISION_FORWARD_RESET	0xFFFFFFFB		// -- 111011
//#define COLLISION_BACKWARD			0x00000008		// -- 001000
//#define COLLISION_BACKWARD_RESET	0xFFFFFFF7		// -- 110111
//
//// -- Z-AXIS
//#define COLLISION_TOP			0x00000010		// -- 010000
//#define COLLISION_TOP_RESET		0xFFFFFFEF		// -- 101111
//#define COLLISION_BOTTOM		0x00000020		// -- 100000
//#define COLLISION_BOTTOM_RESET	0xFFFFFFDF		// -- 011111
//
//// -- Hotspots mode
//enum E_Hotspots_Mode
//{
//	HOTSPOTS_4,
//	HOTSPOTS_8,
//	HOTSPOTS_16
//};
//
//float tileWidth = 1.5f;									    // -- tile's width
//float tileLength = 1.5f;										// -- tile's length
//float tileHeight = 1.5f;									    // -- tile's height
//
//// -- Snaps the x coordinate to its cell
//float SnapToCellX(float coordinate, bool negate, float scale)
//{
//	// -- Offset coordinate with map's position
//	coordinate -= _vPos.x;
//
//	// -- Convert coordinate to NDC
//	coordinate /= _fTileWidth;
//
//	// -- Get scale factor
//	float scaleFactor = scale > 1.f ? scale * 0.5f - 1.f : scale * 0.5f;
//
//	// -- Offset for anchor position
//	float anchor = scaleFactor * (negate ? -1.f : 1.f);
//
//	// -- Get new coordinate
//	coordinate = (float)((int)(coordinate)+0.5f) + anchor;
//
//	// -- Convert to world coordinates
//	coordinate *= _fTileWidth;
//
//	// -- Add back the subtracted offset
//	coordinate += _vPos.x;
//
//	// -- Return new value
//	return coordinate;
//}
//
//// -- Snaps the y coordinate to its cell
//float SnapToCellY(float coordinate, bool negate, float scale) // const??
//{
//	// -- Offset coordinate with map's position
//	coordinate -= _vPos.y;
//
//	// -- Convert coordinate to NDC
//	coordinate /= _fTileHeight;
//
//	// -- Offset for anchor position
//	float anchor = -0.5f;
//
//	// -- Get scale factor
//	float scaleFactor = scale > 1.f ? scale * 0.5f - 1.f : scale * 0.5f;
//
//	// -- Get new coordinate
//	coordinate = (float)((int)(coordinate)+scaleFactor * (negate ? -1 : 1)) + anchor;
//
//	// -- Convert to world coordinates
//	coordinate *= _fTileHeight;
//
//	// -- Add back the subtracted offset
//	coordinate += _vPos.y;
//
//	// -- Return new value
//	return coordinate;
//}
//
//// -- Snaps the z coordinate to its cell
//float SnapToCellZ(float coordinate, bool negate, float scale) // const??
//{
//	// -- Offset coordinate with map's position
//	coordinate -= _vPos.z;
//
//	// -- Convert coordinate to NDC
//	coordinate /= _fTileHeight;
//
//	// -- Offset for anchor position
//	float anchor = -0.5f;
//
//	// -- Get scale factor
//	float scaleFactor = scale > 1.f ? scale * 0.5f - 1.f : scale * 0.5f;
//
//	// -- Get new coordinate
//	coordinate = (float)((int)(coordinate)+scaleFactor * (negate ? -1 : 1)) + anchor;
//
//	// -- Convert to world coordinates
//	coordinate *= _fTileHeight;
//
//	// -- Add back the subtracted offset
//	coordinate += _vPos.z;
//
//	// -- Return new value
//	return coordinate;
//}
//
//// -- Binary map collision detection
//int Map::BinaryCollision(float x, float y, float scaleX, float scaleY)
//{
//	// -- Create initial flag
//	int flag = 0x00000000;
//
//	// -- Get position and scale vectors
//	Vector3f scale = { scaleX / _fTileWidth,
//					   scaleY / _fTileHeight };
//	Vector3f pos = { x, y };
//
//	// -- Offset position with the map's position
//	pos -= _vPos;
//
//	// -- Convert pos to NDC coordinates
//	pos.x /= _fTileWidth;
//	pos.y /= _fTileHeight;
//
//	// -- Offset by 0.5f (since our anchor pt is in the middle)
//	pos += Vector3f(0.5f, -0.5f);
//
//	// -- Negate y position value
//	pos.y *= -1;
//
//	// -- Create hotspot variables
//	float Left1[2], Left2[2],
//		Left3[2], Left4[2],
//		Right1[2], Right2[2],
//		Right3[2], Right4[2],
//		Top1[2], Top2[2],
//		Top3[2], Top4[2],
//		Bot1[2], Bot2[2],
//		Bot3[2], Bot4[2];
//	const int X = 0, Y = 1;
//
//	// -- If obj collider with is >= 1.5x of tile width,
//	// -- create 8 more hotspots (16 total)
//	if (scale.x >= 1.5f || scale.y >= 1.5f)
//	{
//
//		// -- Left hotspot
//		Left1[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left1[Y] = pos.y - scale.y * 0.125f;	// -- To go up 1/8 of the height
//		Left3[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left3[Y] = pos.y - scale.y * 0.375f;	// -- To go up 3/8 of the height
//		Left2[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left2[Y] = pos.y + scale.y * 0.125f;	// -- To go down 1/8 of the height
//		Left4[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left4[Y] = pos.y + scale.y * 0.375f;	// -- To go down 3/8 of the height
//
//		// -- Right hotspot
//		Right1[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right1[Y] = pos.y - scale.y * 0.125f;	// -- To go up 1/8 of the height
//		Right3[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right3[Y] = pos.y - scale.y * 0.375f;	// -- To go up 3/8 of the height
//		Right2[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right2[Y] = pos.y + scale.y * 0.125f;	// -- To go down 1/8 of the height
//		Right4[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right4[Y] = pos.y + scale.y * 0.375f;	// -- To go down 3/8 of the height
//
//		// -- Top hotspot
//		Top1[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top1[X] = pos.x + scale.x * 0.125f;		// -- To go right 1/8 of the width
//		Top3[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top3[X] = pos.x + scale.x * 0.375f;		// -- To go right 3/8 of the width
//		Top2[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top2[X] = pos.x - scale.x * 0.125f;		// -- To go left 1/8 of the width
//		Top4[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top4[X] = pos.x - scale.x * 0.375f;		// -- To go left 3/8 of the width
//
//		// -- Bottom hotspot
//		Bot1[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot1[X] = pos.x + scale.x * 0.125f;		// -- To go right 1/8 of the width
//		Bot3[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot3[X] = pos.x + scale.x * 0.375f;		// -- To go right 3/8 of the width
//		Bot2[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot2[X] = pos.x - scale.x * 0.125f;		// -- To go left 1/8 of the width
//		Bot4[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot4[X] = pos.x - scale.x * 0.375f;		// -- To go left 3/8 of the width
//
//		// -- Check for collision (left)
//		if (GetBinaryCellValue((int)Left1[X], (int)Left1[Y]) ||
//			GetBinaryCellValue((int)Left2[X], (int)Left2[Y]) ||
//			GetBinaryCellValue((int)Left3[X], (int)Left3[Y]) ||
//			GetBinaryCellValue((int)Left4[X], (int)Left4[Y]))
//			flag |= COLLISION_LEFT;
//
//		// -- Check for collision (right)
//		if (GetBinaryCellValue((int)Right1[X], (int)Right1[Y]) ||
//			GetBinaryCellValue((int)Right2[X], (int)Right2[Y]) ||
//			GetBinaryCellValue((int)Right3[X], (int)Right3[Y]) ||
//			GetBinaryCellValue((int)Right4[X], (int)Right4[Y]))
//			flag |= COLLISION_RIGHT;
//
//		// -- Check for collision (top)
//		if (GetBinaryCellValue((int)Top1[X], (int)Top1[Y]) ||
//			GetBinaryCellValue((int)Top2[X], (int)Top2[Y]) ||
//			GetBinaryCellValue((int)Top3[X], (int)Top3[Y]) ||
//			GetBinaryCellValue((int)Top4[X], (int)Top4[Y]))
//			flag |= COLLISION_TOP;
//
//		// -- Check for collision (bottom)
//		if (GetBinaryCellValue((int)Bot1[X], (int)Bot1[Y]) ||
//			GetBinaryCellValue((int)Bot2[X], (int)Bot2[Y]) ||
//			GetBinaryCellValue((int)Bot3[X], (int)Bot3[Y]) ||
//			GetBinaryCellValue((int)Bot4[X], (int)Bot4[Y]))
//			flag |= COLLISION_BOTTOM;
//	}
//
//	// -- Standard 8 hotspots collision check
//	else
//	{
//		// -- Left hotspot
//		Left1[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left1[Y] = pos.y - scale.y * 0.25f;		// -- To go up 1/4 of the height
//		Left2[X] = pos.x - scale.x * 0.5f;		// -- To reach the left side
//		Left2[Y] = pos.y + scale.y * 0.25f;		// -- To go down 1/4 of the height
//
//		// -- Right hotspot
//		Right1[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right1[Y] = pos.y - scale.y * 0.25f;	// -- To go up 1/4 of the height
//		Right2[X] = pos.x + scale.x * 0.5f;		// -- To reach the right side
//		Right2[Y] = pos.y + scale.y * 0.25f;	// -- To go down 1/4 of the height
//
//		// -- Top hotspot
//		Top1[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top1[X] = pos.x + scale.x * 0.25f;		// -- To go right 1/4 of the width
//		Top2[Y] = pos.y - scale.y * 0.5f;		// -- To reach the top side
//		Top2[X] = pos.x - scale.x * 0.25f;		// -- To go left 1/4 of the width
//
//		// -- Bottom hotspot
//		Bot1[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot1[X] = pos.x + scale.x * 0.25f;		// -- To go right 1/4 of the width
//		Bot2[Y] = pos.y + scale.y * 0.5f;		// -- To reach the bottom side
//		Bot2[X] = pos.x - scale.x * 0.25f;		// -- To go left 1/4 of the width
//
//		// -- Check for collision (left)
//		if (GetBinaryCellValue((int)Left1[X], (int)Left1[Y]) ||
//			GetBinaryCellValue((int)Left2[X], (int)Left2[Y]))
//			flag |= COLLISION_LEFT;
//
//		// -- Check for collision (right)
//		if (GetBinaryCellValue((int)Right1[X], (int)Right1[Y]) ||
//			GetBinaryCellValue((int)Right2[X], (int)Right2[Y]))
//			flag |= COLLISION_RIGHT;
//
//		// -- Check for collision (top)
//		if (GetBinaryCellValue((int)Top1[X], (int)Top1[Y]) ||
//			GetBinaryCellValue((int)Top2[X], (int)Top2[Y]))
//			flag |= COLLISION_TOP;
//
//		// -- Check for collision (bottom)
//		if (GetBinaryCellValue((int)Bot1[X], (int)Bot1[Y]) ||
//			GetBinaryCellValue((int)Bot2[X], (int)Bot2[Y]))
//			flag |= COLLISION_BOTTOM;
//	}
//
//	// -- Return collision flag
//	return flag;
//}
//
//// -- Execute map collision with desired game object
//void MapCollision(RigidBody& obj)
//{
//	// -- Get transform reference
//	Transform& transform = obj.GetTransform();
//
//	// -- Get collision flag
//	int flag = BinaryCollision(obj);
//
//	// -- Off ground
//	if (obj._fInitWidth > -1.f && obj._fInitHeight > -1.f)
//	{
//		if ((obj._iMapCollisionFlag & COLLISION_BOTTOM) != COLLISION_BOTTOM && obj.GetTransform()._vVel.y < -500.f)
//		{
//			// -- Set to next tile mod
//			if ((int)obj._fColliderWidth % (int)_fTileWidth != 0)
//				obj._fColliderWidth = static_cast<float>((int)obj._fColliderWidth + ((int)obj._fColliderWidth % (int)_fTileWidth));
//
//			// -- Set to next tile mod
//			if ((int)obj._fColliderHeight % (int)_fTileHeight != 0)
//				obj._fColliderHeight = static_cast<float>((int)obj._fColliderHeight + ((int)obj._fColliderHeight % (int)_fTileHeight));
//		}
//		else
//		{
//			obj._fColliderWidth = obj._fInitWidth;
//			obj._fColliderHeight = obj._fInitHeight;
//		}
//	}
//
//	// -- Get scale factor
//	Vector3f scale = { obj._fColliderWidth / _fTileWidth,
//					   obj._fColliderHeight / _fTileHeight };
//
//	// -- Collision from bottom
//	if ((flag & COLLISION_BOTTOM) == COLLISION_BOTTOM)
//	{
//		// -- Snap position
//		transform._vPos.y = SnapToCellY(transform._vPos.y, false, scale.y);
//
//		// -- Set velocity y to 0
//		transform._vVel.y = 0.f;
//
//		if (obj.GetTransform()._vVel.IsZero() == false)
//			MCEngine::PhysicsEngine::Friction(&obj, obj._Material.GetDynamicFriction());
//	}
//
//	// -- Collision from top
//	if ((flag & COLLISION_TOP) == COLLISION_TOP)
//	{
//		// -- Snap position
//		transform._vPos.y = SnapToCellY(transform._vPos.y, true, scale.y);
//
//		// -- Set velocity y to 0
//		transform._vVel.y = 0.f;
//	}
//
//	// -- Collision from left
//	if ((flag & COLLISION_LEFT) == COLLISION_LEFT)
//	{
//		// -- Snap position
//		transform._vPos.x = SnapToCellX(transform._vPos.x, false, scale.x);
//
//		// -- Set velocity x to 0
//		transform._vVel.x = 0.f;
//	}
//
//	// -- Collision from right
//	if ((flag & COLLISION_RIGHT) == COLLISION_RIGHT)
//	{
//		// -- Snap position
//		transform._vPos.x = SnapToCellX(transform._vPos.x, true, scale.x);
//
//		// -- Set velocity x to 0
//		transform._vVel.x = 0.f;
//	}
//
//	// -- Set flag of rb
//	obj._iMapCollisionFlag = flag;
//}
//
//// -- Returns the binary cell value
//bool GetBinaryCellValue(int x, int y)
//{
//	// -- Out of bounds check
//	if (x < 0.f || y < 0.f ||
//		x >= m_iMapWidth || y >= m_iMapHeight)
//		return false;
//
//	// -- Return cell value
//	return m_pBinaryDataArr[y][x];
//}
//
//// -- Hotspots collision check
//int PhysicsEngine::Collision_Hotspots(const RigidBody& rb1, const RigidBody& rb2, E_Hotspots_Mode mode)
//{
//	// -- Inactive rigid body, return false
//	if (!rb1.IsActive() || !rb2.IsActive())
//		return 0;
//
//	// -- Check for bb component
//	if (!rb2.GetObj()->HasComponent<BoundingBox>())
//		return 0;
//
//	// -- Declare initial flag
//	int flag = 0;
//
//	// -- Get width & height values
//	float width = rb1.GetWidth(), height = rb1.GetHeight();
//
//	// -- Do 4 hotspots
//	if (mode == HOTSPOTS_4)
//	{
//		// -- Get hotspots of rb1
//		Vector3f hotspots[4];
//
//		// -- Top hotspots
//		hotspots[0].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[0].x = rb1.GetTransform()._vPos.x;
//
//		// -- Right hotspots
//		hotspots[1].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[1].y = rb1.GetTransform()._vPos.y;
//
//		// -- Bottom hotspots
//		hotspots[2].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[2].x = rb1.GetTransform()._vPos.x;
//
//		// -- Left hotspots
//		hotspots[3].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[3].y = rb1.GetTransform()._vPos.y;
//
//		// -- Get bounding box
//		const BoundingBox& bb = rb2.GetObj()->GetComponent<BoundingBox>();
//
//		// -- Check for top collision
//		if (Collision_PtBox(hotspots[0], bb))
//			flag |= COLLISION_TOP;
//
//		// -- Check for right collision
//		if (Collision_PtBox(hotspots[1], bb))
//			flag |= COLLISION_RIGHT;
//
//		// -- Check for bottom collision
//		if (Collision_PtBox(hotspots[2], bb))
//			flag |= COLLISION_BOTTOM;
//
//		// -- Check for left collision
//		if (Collision_PtBox(hotspots[3], bb))
//			flag |= COLLISION_LEFT;
//	}
//
//	// -- Do 8 hotspots
//	else if (mode == HOTSPOTS_8)
//	{
//		// -- Get hotspots of rb1
//		Vector3f hotspots[8];
//
//		// -- Top hotspots
//		hotspots[0].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[0].x = rb1.GetTransform()._vPos.x - width * 0.25f;
//		hotspots[1].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[1].x = rb1.GetTransform()._vPos.x + width * 0.25f;
//
//		// -- Right hotspots
//		hotspots[2].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[2].y = rb1.GetTransform()._vPos.y + height * 0.25f;
//		hotspots[3].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[3].y = rb1.GetTransform()._vPos.y - height * 0.25f;
//
//		// -- Bottom hotspots
//		hotspots[4].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[4].x = rb1.GetTransform()._vPos.x - width * 0.25f;
//		hotspots[5].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[5].x = rb1.GetTransform()._vPos.x + width * 0.25f;
//
//		// -- Left hotspots
//		hotspots[6].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[6].y = rb1.GetTransform()._vPos.y + height * 0.25f;
//		hotspots[7].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[7].y = rb1.GetTransform()._vPos.y - height * 0.25f;
//
//		// -- Get bounding box
//		const BoundingBox& bb = rb2.GetObj()->GetComponent<BoundingBox>();
//
//		// -- Check for top collision
//		if (Collision_PtBox(hotspots[0], bb) || Collision_PtBox(hotspots[1], bb))
//			flag |= COLLISION_TOP;
//
//		// -- Check for right collision
//		if (Collision_PtBox(hotspots[2], bb) || Collision_PtBox(hotspots[3], bb))
//			flag |= COLLISION_RIGHT;
//
//		// -- Check for bottom collision
//		if (Collision_PtBox(hotspots[4], bb) || Collision_PtBox(hotspots[5], bb))
//			flag |= COLLISION_BOTTOM;
//
//		// -- Check for left collision
//		if (Collision_PtBox(hotspots[6], bb) || Collision_PtBox(hotspots[7], bb))
//			flag |= COLLISION_LEFT;
//	}
//
//	// -- Do 16 hotspots
//	else
//	{
//		// -- Get hotspots of rb1
//		Vector3f hotspots[16];
//
//		// -- Top hotspots
//		hotspots[0].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[0].x = rb1.GetTransform()._vPos.x - width * 0.125f;
//		hotspots[1].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[1].x = rb1.GetTransform()._vPos.x - width * 0.375f;
//		hotspots[2].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[2].x = rb1.GetTransform()._vPos.x + width * 0.125f;
//		hotspots[3].y = rb1.GetTransform()._vPos.y + height * 0.5f;
//		hotspots[3].x = rb1.GetTransform()._vPos.x + width * 0.375f;
//
//		// -- Right hotspots
//		hotspots[4].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[4].y = rb1.GetTransform()._vPos.y - height * 0.125f;
//		hotspots[5].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[5].y = rb1.GetTransform()._vPos.y - height * 0.375f;
//		hotspots[6].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[6].y = rb1.GetTransform()._vPos.y + height * 0.125f;
//		hotspots[7].x = rb1.GetTransform()._vPos.x + width * 0.5f;
//		hotspots[7].y = rb1.GetTransform()._vPos.y + height * 0.375f;
//
//		// -- Bottom hotspots
//		hotspots[8].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[8].x = rb1.GetTransform()._vPos.x - width * 0.125f;
//		hotspots[9].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[9].x = rb1.GetTransform()._vPos.x - width * 0.375f;
//		hotspots[10].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[10].x = rb1.GetTransform()._vPos.x + width * 0.125f;
//		hotspots[11].y = rb1.GetTransform()._vPos.y - height * 0.5f;
//		hotspots[11].x = rb1.GetTransform()._vPos.x + width * 0.375f;
//
//		// -- Left hotspots
//		hotspots[12].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[12].y = rb1.GetTransform()._vPos.y - height * 0.125f;
//		hotspots[13].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[13].y = rb1.GetTransform()._vPos.y - height * 0.375f;
//		hotspots[14].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[14].y = rb1.GetTransform()._vPos.y + height * 0.125f;
//		hotspots[15].x = rb1.GetTransform()._vPos.x - width * 0.5f;
//		hotspots[15].y = rb1.GetTransform()._vPos.y + height * 0.375f;
//
//		// -- Get bounding box
//		const BoundingBox& bb = rb2.GetObj()->GetComponent<BoundingBox>();
//
//		// -- Check for top collision
//		if (Collision_PtBox(hotspots[0], bb) || Collision_PtBox(hotspots[1], bb) ||
//			Collision_PtBox(hotspots[2], bb) || Collision_PtBox(hotspots[3], bb))
//			flag |= COLLISION_TOP;
//
//		// -- Check for right collision
//		if (Collision_PtBox(hotspots[4], bb) || Collision_PtBox(hotspots[5], bb) ||
//			Collision_PtBox(hotspots[6], bb) || Collision_PtBox(hotspots[7], bb))
//			flag |= COLLISION_RIGHT;
//
//		// -- Check for bottom collision
//		if (Collision_PtBox(hotspots[8], bb) || Collision_PtBox(hotspots[9], bb) ||
//			Collision_PtBox(hotspots[10], bb) || Collision_PtBox(hotspots[11], bb))
//			flag |= COLLISION_BOTTOM;
//
//		// -- Check for left collision
//		if (Collision_PtBox(hotspots[12], bb) || Collision_PtBox(hotspots[13], bb) ||
//			Collision_PtBox(hotspots[14], bb) || Collision_PtBox(hotspots[15], bb))
//			flag |= COLLISION_LEFT;
//	}
//
//	// -- Return collision flag
//	return flag;
//}

#endif