#pragma once
#ifndef _PATHSYSTEM_H_
#define _PATHSYSTEM_H_

#include "DebugSystem.h"
#include "UISystem.h"
#include "../Components/Path.h"
#include "Math/Math_includes.h"
#include <algorithm>
#include <map>

struct path_system : paperback::system::instance
{
	// System Ptrs
	debug_system* debug_sys;
	ui_system* ui_sys;

	// Queries
	tools::query Query_Paths;
	tools::query Query_Units;

	bool m_MovePathFollowers{};

	// Map of splines
	std::map<int, paperback::Spline> splines;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "path_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		debug_sys = &GetSystem<debug_system>();
		ui_sys = &GetSystem<ui_system>();

		Query_Paths.m_Must.AddFromComponents<path, transform>();
		Query_Paths.m_NoneOf.AddFromComponents<prefab>();

		Query_Units.m_Must.AddFromComponents<rigidforce, rigidbody, path_follower, transform, rotation>();
		Query_Units.m_NoneOf.AddFromComponents<prefab>();
	}

	PPB_INLINE
	void OnStateChange( void ) noexcept
	{
		//splines.clear();
	}

	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
		/*ForEach(Search(Query_Paths), [&]( path& Path, transform& Transform ) noexcept
		{
			std::vector<paperback::Spline::SplinePoint> spline_points;
		
			for (auto& point : Path.m_Points)
			{
				spline_points.push_back({ Transform.m_Position + point });
			}
		
			splines.emplace(Path.m_ID, paperback::Spline{ spline_points, false });
		});*/
	}

	PPB_FORCEINLINE
		void OnPause(const bool& Status) noexcept	//function activates whenever play / pause is clicked
	{
		m_bPaused = Status;

		if (!Status) //is paused, play is clicked
		{
			m_MovePathFollowers = true;
		}

		else
		{
			m_MovePathFollowers = false;
		}
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		ForEach(Search(Query_Paths), [&](path& Path, transform& Transform) noexcept
		{
			std::vector<paperback::Spline::SplinePoint> spline_points;

			for (auto& point : Path.m_Points)
			{
				spline_points.push_back({ Transform.m_Position + point });
			}

			splines.emplace(Path.m_ID, paperback::Spline{ spline_points, false });
		});

		ForEach(Search(Query_Paths), [&]( path& Path ) noexcept
		{
			if (debug_sys->m_IsDebug)
			{
				debug_sys->DrawSpline(splines[Path.m_ID]);
			}
		});

		if (m_MovePathFollowers)
		{
			ForEach(Search(Query_Units), [&](rigidforce& Rigidforce, rigidbody& Rigidbody, path_follower& PathFollower, transform& Transform, rotation& Rotation) noexcept
			{
				auto spline = splines.find(PathFollower.m_PathID);

				if (spline != splines.end() && !PathFollower.m_PauseTravel)
				{
					if (PathFollower.m_TravelSpeed <= 0.0f)
					{
						Rigidforce.m_Momentum = { 0.0f, 0.0f, 0.0f };
					}

					else if (!PathFollower.m_Reversed && PathFollower.m_Distance >= spline->second.m_TotalLength ||
							  PathFollower.m_Reversed && PathFollower.m_Distance <= 0.0f)
					{
						PathFollower.m_FinishedTravelling = true;

						if (PathFollower.m_BackAndForth)
						{
							PathFollower.m_Reversed = !PathFollower.m_Reversed;
							Movement(spline->second, Rigidforce, Rigidbody, PathFollower, Transform, Rotation);
						}

						else
						{
							Rigidforce.m_Momentum = { 0.0f, 0.0f, 0.0f };
						}
					}

					else
					{
						PathFollower.m_FinishedTravelling = false;
						Movement(spline->second, Rigidforce, Rigidbody, PathFollower, Transform, Rotation);
					}
				}

				else if (PathFollower.m_PauseTravel)
				{
					Rigidforce.m_Momentum = { 0.0f, 0.0f, 0.0f };
				}
			});
		}

		splines.clear();
	}

	void Movement(paperback::Spline& spline, rigidforce& Rigidforce, rigidbody& Rigidbody, path_follower& PathFollower, transform& Transform, rotation& Rotation)
	{
		float normalized_offset{ spline.GetNormalizedOffset(PathFollower.m_Distance) };

		paperback::Vector3f destination{ spline.GetSplinePoint(normalized_offset).m_Point };
		paperback::Vector3f gradient{ spline.GetSplineGradient(normalized_offset).Normalized() };
		paperback::Vector3f direction{ (destination - Transform.m_Position) };
		paperback::Vector3f norm_direction{ direction.Normalized() };

		float temp{ direction.Magnitude() * direction.Magnitude() * direction.Magnitude() };
		float speed_modifier{ std::min(1.0f, 1.0f / temp) };

		Rigidforce.m_Momentum = direction * PathFollower.m_TravelSpeed;

		if (PathFollower.m_Reversed)
		{
			PathFollower.m_Distance -= speed_modifier * PathFollower.m_TravelSpeed * m_Coordinator.DeltaTime();
		}

		else
		{
			PathFollower.m_Distance += speed_modifier * PathFollower.m_TravelSpeed * m_Coordinator.DeltaTime();
		}

		std::vector<paperback::Vector3f> vec;
		vec.push_back(destination);
		vec.push_back(Transform.m_Position);
		
		if (debug_sys->m_IsDebug)
		{
			debug_sys->DrawDebugLines(vec, true);
		}
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

//	void OnEvent(const size_t& Key, const bool& Clicked) noexcept
//	{
//		if (!Clicked && Key == GLFW_MOUSE_BUTTON_1 && ui_sys->m_Picked)
//		{
//			//lane selection
//			struct lane_box
//			{
//				paperback::Vector3f m_Min;
//				paperback::Vector3f m_Max;
//				paperback::Vector3f m_Position;
//				int m_Lane;
//			};
//
//			std::vector<lane_box> lane_boxes;
//
//			for (int i = 0 ; i < splines.size() ; ++i)
//			{
//				float interval{ 3.0f };
//
//				for (float normalized_offset = 0.0f, distance = 0.0f;
//					 normalized_offset < static_cast<float>(splines[i].m_Points.size()) - 3.5f;
//					 distance += interval)
//				{
//					paperback::Vector3f box_point{ splines[i].GetSplinePoint(normalized_offset).m_Point };
//					paperback::Vector3f min{ -2.3f, -2.3f, -2.3f };
//					paperback::Vector3f max{ -min };
//					lane_boxes.push_back({ min, max, box_point, i });
//					normalized_offset = splines[i].GetNormalizedOffset(distance);
//				}
//			}
//
//			int lane{-1};
//			glm::vec3 CamPos{ cam::GetInstanced().GetPosition() };
//			glm::vec3 RayDir{ PPB.GetMousePosition() };
//
//			for (auto lane_box : lane_boxes)
//			{
//				float t = 0.0f;
//
//				if (RayAabb({ CamPos.x, CamPos.y, CamPos.z },
//							{ RayDir.x, RayDir.y, RayDir.z },
//							lane_box.m_Position + lane_box.m_Min, lane_box.m_Position + lane_box.m_Max, t))
//				{
//					lane = lane_box.m_Lane;
//					break;
//				}
//			}
//
//			ForEach(Search(Query_Paths), [&]( path& Path, selected& Selected, mesh& Mesh ) noexcept
//			{
//				if (Path.m_ID == lane)
//				{
//					Selected.m_Value = true;
//					Mesh.m_Active = true;
//				}
//
//				else
//				{
//					Selected.m_Value = false;
//					Mesh.m_Active = false;
//				}
//			});
//
//			if (debug_sys->m_IsDebug)
//			{
//				for (auto& lane_box : lane_boxes)
//				{
//					if (lane_box.m_Lane == lane)
//					{
//						boundingbox cube{ lane_box.m_Min, lane_box.m_Max, true };
//						debug_sys->DrawCube(cube, lane_box.m_Position);
//					}
//
//					else
//					{
//						boundingbox cube{ lane_box.m_Min, lane_box.m_Max, false };
//						debug_sys->DrawCube(cube, lane_box.m_Position);
//					}
//				}
//			}
//		}
//
//		else if (Clicked)
//		{
//			ForEach(Search(Query_Paths), [&]( selected& Selected, mesh& Mesh ) noexcept
//			{
//				Selected.m_Value = false;
//				Mesh.m_Active = false;
//			});
//		}
//	}
};

#endif
