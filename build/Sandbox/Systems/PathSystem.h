#pragma once
#ifndef _PATHSYSTEM_H_
#define _PATHSYSTEM_H_

#include "DebugSystem.h"
#include "../Components/Path.h"
#include "Math/Math_includes.h"
#include <algorithm>
#include <map>

struct path_system : paperback::system::instance
{
	debug_system* debug_sys;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "path_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		debug_sys = &GetSystem<debug_system>();
	}

	PPB_FORCEINLINE
	void PreUpdate(void) noexcept
	{
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		std::map<int, paperback::Spline> splines;

		tools::query Query_Paths;
		Query_Paths.m_Must.AddFromComponents<path, transform>();

		ForEach(Search(Query_Paths), [&](path& Path, transform& Transform) noexcept
		{
			std::vector<paperback::Spline::SplinePoint> spline_points;

			for (auto& point : Path.m_Points)
			{
				spline_points.push_back({ Transform.m_Position + point });
			}

			splines[Path.m_ID] = { spline_points, false };
				
			if (debug_sys->m_IsDebug)
			{
				debug_sys->DrawSpline(splines[Path.m_ID]);
			}
		});

		tools::query Query_Units;
		Query_Units.m_Must.AddFromComponents<rigidforce, rigidbody, path_follower, transform, rotation>();
		Query_Units.m_OneOf.AddFromComponents<friendly, enemy>();
		Query_Units.m_NoneOf.AddFromComponents<prefab>();

		ForEach(Search(Query_Units), [&](rigidforce& Rigidforce, rigidbody& Rigidbody, path_follower& PathFollower, transform& Transform, rotation& Rotation, friendly* Friendly, enemy* Enemy) noexcept
		{
			auto spline = splines.find(PathFollower.m_ID);

			if (spline != splines.end())
			{

				if (PathFollower.m_Distance >= spline->second.m_TotalLength || PathFollower.m_TravelSpeed <= 0.0f)
				{
					Rigidforce.m_Momentum = { 0.0f, 0.0f, 0.0f };
				}

				else
				{
					float normalized_offset{ spline->second.GetNormalizedOffset(PathFollower.m_Distance) };

					//Enemies start from the end of the path
					if (Enemy)
					{
						normalized_offset = (spline->second.m_Points.size() - 3.01f) - normalized_offset;
					}

					paperback::Vector3f destination{ spline->second.GetSplinePoint(normalized_offset).m_Point };
					paperback::Vector3f gradient{ spline->second.GetSplineGradient(normalized_offset).Normalized() };
					paperback::Vector3f direction{ (destination - Transform.m_Position).Normalized() };
					
					//Rotation.m_Value += GetRotationAngles(PathFollower.m_Direction, direction);
					PathFollower.m_Direction = direction;

					float temp{ direction.Magnitude() * direction.Magnitude() * 6.0f };
					float speed_modifier{ std::min(1.0f, 1.0f / temp) };
					float climb_modifier{};

					direction.y > 0 ? climb_modifier = 1.0f - std::min(0.7f, direction.y) :
						climb_modifier = 1.0f + std::min(0.7f, abs(direction.y));

					Rigidforce.m_Momentum = direction * PathFollower.m_TravelSpeed * climb_modifier;
					PathFollower.m_Distance += speed_modifier * PathFollower.m_TravelSpeed * climb_modifier * m_Coordinator.DeltaTime();

					std::vector<paperback::Vector3f> vec;
					vec.push_back(destination);
					vec.push_back(Transform.m_Position);
					debug_sys->DrawDebugLines(vec, true);
				}
			}
		});
	}

	paperback::Vector3f GetRotationAngles(paperback::Vector3f vec1, paperback::Vector3f vec2)
	{
		paperback::Vector3f axis_of_rot{ vec1.Cross(vec2) };
		float angle_of_rot{ vec1.Angle(vec2) };

		if (axis_of_rot == paperback::Vector3f{} || axis_of_rot == paperback::Vector3f{180.0f, 180.0f, 180.0f} || angle_of_rot == 0.0f || std::isnan(angle_of_rot))
		{
			return {};
		}

		float qw{ cos(angle_of_rot / 2.0f) };
		float qx{ axis_of_rot.x * sin(angle_of_rot / 2.0f) };
		float qy{ axis_of_rot.y * sin(angle_of_rot / 2.0f) };
		float qz{ axis_of_rot.z * sin(angle_of_rot / 2.0f) };

		float heading{ atan2(2 * qy * qw - 2 * qx * qz, 1 - 2 * qy * qy - 2 * qz * qz) };
		float attitude{ asin(2 * qx * qy + 2 * qz * qw) };
		float bank{ atan2(2 * qx * qw - 2 * qy * qz, 1 - 2 * qx * qx - 2 * qz * qz) };

		return { {std::isnan(heading) ? 0.0f : heading},
				 {std::isnan(bank) ? 0.0f : bank},
				 {std::isnan(attitude) ? 0.0f : attitude } };
	}
};

#endif