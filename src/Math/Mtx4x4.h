/**********************************************************************************
*\file         Mtx4x4.h
*\brief        Mtx4x4.h
*
*\author	   Lee Jian Yan, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef MTX4X4_H
#define MTX4X4_H

#include "Vector3f.h"
#include "MathUtils.h"

namespace paperback
{
#define MTX_TOTAL_ELEMENTS 16

	union Mtx4x4
	{
		float m[MTX_TOTAL_ELEMENTS];					// -- Arr of 16 floats [shared mem]
		float m2[4][4];									// -- Arr of 4x4 floats [shared mem]

		//-------------------------------------------------------------------------------
		Mtx4x4()										// -- Default constructor
		{
			for (int i = 0; i < MTX_TOTAL_ELEMENTS; ++i)
				m[i] = 0.f;
		}

		Mtx4x4(float* _pArr)							// -- Constructs a Mtx4x4 object via a float arr
			: m()
		{
			// -- Populate float array
			for (int i = 0; i < MTX_TOTAL_ELEMENTS; ++i)
				m[i] = _pArr[i];
		}

		// -- m00, m01, m02, m03
		// -- m10, m11, m12, m13
		// -- m20, m21, m22, m23
		// -- m30, m31, m32, m33
		Mtx4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			// -- Populate float arr
			m2[0][0] = m00, m2[0][1] = m01, m2[0][2] = m02, m2[0][3] = m03;
			m2[1][0] = m10, m2[1][1] = m11, m2[1][2] = m12, m2[1][3] = m13;
			m2[2][0] = m20, m2[2][1] = m21, m2[2][2] = m22, m2[2][3] = m23;
			m2[3][0] = m30, m2[3][1] = m31, m2[3][2] = m32, m2[3][3] = m33;
		}

		Mtx4x4(const Mtx4x4 &rhs)							// -- Copy constructor
			: m()
		{
			// -- Copy values
			for (int i = 0; i < MTX_TOTAL_ELEMENTS; ++i)
				m[i] = rhs.m[i];
		}

		inline Mtx4x4& operator=(const Mtx4x4&);		// -- Assignment operator

		//-------------------------------------------------------------------------------
		Mtx4x4& operator*=(const Mtx4x4& rhs)			// -- operator*=
		{
			// -- Create float array to store results
			float arr[MTX_TOTAL_ELEMENTS];

			// -- Compute Mtx4x4 multiplication [row 1]
			arr[0] = m2[0][0] * rhs.m2[0][0] + m2[0][1] * rhs.m2[1][0] + m2[0][2] * rhs.m2[2][0] + m2[0][3] * rhs.m2[3][0];
			arr[1] = m2[0][0] * rhs.m2[0][1] + m2[0][1] * rhs.m2[1][1] + m2[0][2] * rhs.m2[2][1] + m2[0][3] * rhs.m2[3][1];
			arr[2] = m2[0][0] * rhs.m2[0][2] + m2[0][1] * rhs.m2[1][2] + m2[0][2] * rhs.m2[2][2] + m2[0][3] * rhs.m2[3][2];
			arr[3] = m2[0][0] * rhs.m2[0][3] + m2[0][1] * rhs.m2[1][3] + m2[0][2] * rhs.m2[2][3] + m2[0][3] * rhs.m2[3][3];

			// -- Compute Mtx4x4 multiplication [row 2]
			arr[4] = m2[1][0] * rhs.m2[0][0] + m2[1][1] * rhs.m2[1][0] + m2[1][2] * rhs.m2[2][0] + m2[1][3] * rhs.m2[3][0];
			arr[5] = m2[1][0] * rhs.m2[0][1] + m2[1][1] * rhs.m2[1][1] + m2[1][2] * rhs.m2[2][1] + m2[1][3] * rhs.m2[3][1];
			arr[6] = m2[1][0] * rhs.m2[0][2] + m2[1][1] * rhs.m2[1][2] + m2[1][2] * rhs.m2[2][2] + m2[1][3] * rhs.m2[3][2];
			arr[7] = m2[1][0] * rhs.m2[0][3] + m2[1][1] * rhs.m2[1][3] + m2[1][2] * rhs.m2[2][3] + m2[1][3] * rhs.m2[3][3];

			// -- Compute Mtx4x4 multiplication [row 3]
			arr[8] = m2[2][0] * rhs.m2[0][0] + m2[2][1] * rhs.m2[1][0] + m2[2][2] * rhs.m2[2][0] + m2[2][3] * rhs.m2[3][0];
			arr[9] = m2[2][0] * rhs.m2[0][1] + m2[2][1] * rhs.m2[1][1] + m2[2][2] * rhs.m2[2][1] + m2[2][3] * rhs.m2[3][1];
			arr[10] = m2[2][0] * rhs.m2[0][2] + m2[2][1] * rhs.m2[1][2] + m2[2][2] * rhs.m2[2][2] + m2[2][3] * rhs.m2[3][2];
			arr[11] = m2[2][0] * rhs.m2[0][3] + m2[2][1] * rhs.m2[1][3] + m2[2][2] * rhs.m2[2][3] + m2[2][3] * rhs.m2[3][3];

			// -- Compute Mtx4x4 multiplication [row 4]
			arr[12] = m2[3][0] * rhs.m2[0][0] + m2[3][1] * rhs.m2[1][0] + m2[3][2] * rhs.m2[2][0] + m2[3][3] * rhs.m2[3][0];
			arr[13] = m2[3][0] * rhs.m2[0][1] + m2[3][1] * rhs.m2[1][1] + m2[3][2] * rhs.m2[2][1] + m2[3][3] * rhs.m2[3][1];
			arr[14] = m2[3][0] * rhs.m2[0][2] + m2[3][1] * rhs.m2[1][2] + m2[3][2] * rhs.m2[2][2] + m2[3][3] * rhs.m2[3][2];
			arr[15] = m2[3][0] * rhs.m2[0][3] + m2[3][1] * rhs.m2[1][3] + m2[3][2] * rhs.m2[2][3] + m2[3][3] * rhs.m2[3][3];

			// -- Create a Mtx4x4 with the float arr constructor
			// -- and assign it to *this via the assignment operator
			*this = Mtx4x4(arr);

			// -- Return ref to this
			return *this;
		}

		inline Mtx4x4 operator*(const Mtx4x4&);		// -- operator*
		inline Vector3f operator*(const Vector3f&);	// -- Multiplies a matrix with a vector

		//-------------------------------------------------------------------------------
		static inline Mtx4x4 Identity();				// -- Returns the identity matrix
		static inline Mtx4x4 Trans(const Vector3f&);	// -- Returns the translation matrix
		static inline Mtx4x4 Scale(const Vector3f&);	// -- Returns the scale matrix
		static inline Mtx4x4 RotX(float angle);			// -- Returns the rotation matrix in the X axis
		static inline Mtx4x4 RotY(float angle);			// -- Returns the rotation matrix in the Y axis
		static inline Mtx4x4 RotZ(float angle);			// -- Returns the rotation matrix in the Z axis
		static inline Mtx4x4 RotRad2D(float angle)		// -- Returns the rotation matrix (Z axis) taking in an angle in radians
		{
			//return RotZ(angle);
			return Mtx4x4{};
		}
		static inline Mtx4x4 RotDeg2D(float angle)		// -- Returns the rotation matrix (Z axis) taking in an angle in degrees
		{
			//return RotZ(MathUtils::DegToRad(angle));
			return Mtx4x4{};
		}
		static Mtx4x4 LookAt(const Vector3f& up,		// -- Gets the LookAt matrix, taking in the up vector of the camera,
			const Vector3f& target,	// -- target pos of the camera
			const Vector3f& pos)		// -- and the camera's position.
		{
			// -- Calculate direction of camera
			Vector3f dir = (pos - target).Normalized();

			// -- Calculate right vector of camera
			Vector3f right = Vector3f(0.f, 1.f, 0.f).Cross(dir).Normalized();

			// -- Create the lookat matrix
			Mtx4x4 Result = Mtx4x4(right.x, right.y, right.z, 0.f,
				up.x, up.y, up.z, 0.f,
				dir.x, dir.y, dir.z, 0.f,
				0.f, 0.f, 0.f, 1.f) * Trans(-pos);

			// -- Return result
			return Result;
		}
	};
}

// -- Mtx4x4 hpp include
#include "Mtx4x4.hpp"

// -- End of header guard MTX4X4_H
#endif

