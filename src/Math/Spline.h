#pragma once
#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include "Vector3f.h"

namespace paperback
{
	struct Spline
	{
		struct SplinePoint
		{
			Vector3f m_Point;
			float m_Length;
		};

		Spline() = default;

		Spline(std::vector<SplinePoint> points, bool looped)
			: m_Points {points},
			  m_Looped {looped},
			  m_TotalLength {}
		{
			for (int i = 0; i < m_Points.size() - 3; ++i)
			{
				float length { CalculateSegmentLength(i) };
				m_Points[i].m_Length = length;
				m_TotalLength += length;
			}
		}

		SplinePoint GetSplinePoint(float t)
		{
			if (m_Points.size() >= 4)
			{
				int p0, p1, p2, p3;

				if (!m_Looped)
				{
					p0 = static_cast<int>(t);
					p1 = p0 + 1;
					p2 = p1 + 1;
					p3 = p2 + 1;
				}

				else
				{
					p1 = static_cast<int>(t);
					p2 = (p1 + 1) % m_Points.size();
					p3 = (p2 + 1) % m_Points.size();
					p0 = p1 >= 1 ? p1 - 1 : static_cast<int>(m_Points.size()) - 1;
				}
				
				t = t - static_cast<int>(t);

				float tt = t * t;
				float ttt = t * t * t;

				float q0 = -ttt + 2.0f * tt - t;
				float q1 = 3.0f * ttt - 5.0f * tt + 2.0f;
				float q2 = -3.0f * ttt + 4.0f * tt + t;
				float q3 = ttt - tt;

				return { 0.5f * (m_Points[p0].m_Point * q0 + m_Points[p1].m_Point * q1 + m_Points[p2].m_Point * q2 + m_Points[p3].m_Point * q3), 0.0f };
			}

			return {};
		}

		Vector3f GetSplineGradient(float t)
		{
			if (m_Points.size() >= 4)
			{
				int p0, p1, p2, p3;

				if (!m_Looped)
				{
					p0 = static_cast<int>(t);
					p1 = p0 + 1;
					p2 = p1 + 1;
					p3 = p2 + 1;
				}

				else
				{
					p1 = static_cast<int>(t);
					p2 = (p1 + 1) % m_Points.size();
					p3 = (p2 + 1) % m_Points.size();
					p0 = p1 >= 1 ? p1 - 1 : static_cast<int>(m_Points.size()) - 1;
				}

				t = t - static_cast<int>(t);

				float tt = t * t;
				float ttt = t * t * t;

				float q0 = -3.0f * tt + 4.0f * t - 1;
				float q1 = 9.0f * tt - 10.0f * t;
				float q2 = -9.0f * tt + 8.0f * t + 1.0f;
				float q3 = 3.0f * tt - 2.0f * t;
				
				return 0.5f * (m_Points[p0].m_Point * q0 + m_Points[p1].m_Point * q1 + m_Points[p2].m_Point * q2 + m_Points[p3].m_Point * q3);
			}

			return {};
		}

		float CalculateSegmentLength(int node)
		{
			float length{};

			SplinePoint old_point{ GetSplinePoint(static_cast<float>(node)) };

			for (float t = 0; t < 1.0f; t += 0.05f)
			{
				SplinePoint new_point = GetSplinePoint(static_cast<float>(node) + t);
				length += sqrtf((new_point.m_Point.x - old_point.m_Point.x) * (new_point.m_Point.x - old_point.m_Point.x) +
								(new_point.m_Point.y - old_point.m_Point.y) * (new_point.m_Point.y - old_point.m_Point.y) +
								(new_point.m_Point.z - old_point.m_Point.z) * (new_point.m_Point.z - old_point.m_Point.z));

				old_point = new_point;
			}

			return length;
		}

		float GetNormalizedOffset(float p)
		{
			int i{};

			for (; p > m_Points[i].m_Length; ++i)
			{
				p -= m_Points[i].m_Length;
			}

			return static_cast<float>(i + p / m_Points[i].m_Length);
		}

		std::vector<SplinePoint> m_Points;
		bool m_Looped;
		float m_TotalLength;
	};
}

#endif