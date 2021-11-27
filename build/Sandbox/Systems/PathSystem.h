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
		//temporary
		std::vector<paperback::Spline::SplinePoint> spline_points;
		spline_points.push_back({ { -5.0f, 2.0f, -5.0f } });
		spline_points.push_back({ { -3.0f, 5.0f, -6.0f } });
		spline_points.push_back({ { 4.0f, 2.0f, -6.0f } });
		spline_points.push_back({ { 7.0f, 5.0f, -5.0f } });
		spline_points.push_back({ { 7.5f, 5.5f, -4.0f } });
		spline_points.push_back({ { 8.0f, 3.0f, -5.0f } });
		spline_points.push_back({ { 10.0f, 4.0f, -4.0f } });
		paperback::Spline spline{ spline_points, false };

		std::map<int, paperback::Spline> splines;

		tools::query Query_Paths;
		Query_Paths.m_Must.AddFromComponents<path, transform>();

		if (debug_sys->m_IsDebug)
		{
			debug_sys->DrawSpline(spline);

			ForEach(Search(Query_Paths), [&](path& Path, transform& Transform) noexcept
			{
				std::vector<paperback::Spline::SplinePoint> spline_points;

				for (auto& point : Path.m_Points)
				{
					spline_points.push_back({ Transform.m_Position + point });
				}

				splines[Path.m_ID] = { spline_points, false };
				debug_sys->DrawSpline(splines[Path.m_ID]);
			});
		}

		tools::query Query_Units;
		Query_Units.m_Must.AddFromComponents<rigidforce, rigidbody, path_follower, transform>();
		Query_Units.m_OneOf.AddFromComponents<friendly, enemy>();
		Query_Units.m_NoneOf.AddFromComponents<prefab>();

		ForEach(Search(Query_Units), [&](rigidforce& Rigidforce, rigidbody& Rigidbody, path_follower& PathFollower, transform& Transform, friendly* Friendly, enemy* Enemy) noexcept
		{
			if (PathFollower.m_Distance >= spline.m_TotalLength || PathFollower.m_TravelSpeed <= 0.0f)
			{
				Rigidforce.m_Momentum = { 0.0f, 0.0f, 0.0f };
			}

			else
			{
				float normalized_offset{ spline.GetNormalizedOffset(PathFollower.m_Distance) };

				//Enemies start from the end of the path
				if (Enemy)
				{
					normalized_offset = (spline.m_Points.size() - 3.01f) - normalized_offset;
				}

				paperback::Vector3f destination{ spline.GetSplinePoint(normalized_offset).m_Point };
				paperback::Vector3f gradient{ spline.GetSplineGradient(normalized_offset).Normalized() };
				paperback::Vector3f direction{ (destination - Transform.m_Position) };

				float temp{ direction.Magnitude() * direction.Magnitude() * 100.0f };
				float speed_modifier{ std::min(1.0f, 1.0f / temp) };
				float climb_modifier{};

				direction.y > 0 ? climb_modifier = 1.0f - std::min(0.7f, direction.y) :
					climb_modifier = 1.0f + std::min(0.7f, abs(direction.y));

				Rigidforce.m_Momentum = direction.Normalized() * PathFollower.m_TravelSpeed * climb_modifier;
				PathFollower.m_Distance += speed_modifier * PathFollower.m_TravelSpeed * climb_modifier * m_Coordinator.DeltaTime();

				std::vector<paperback::Vector3f> vec;
				vec.push_back(destination);
				vec.push_back(Transform.m_Position);
				debug_sys->DrawDebugLines(vec, true);

				auto it = splines.find(PathFollower.m_ID);

				if (it != splines.end())
				{
					//use that spline
				}
			}
		});
	}
};

#endif