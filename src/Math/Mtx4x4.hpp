#include "MathUtils.h"

namespace paperback
{
	//------ Returns a rotated vector by an angle (radians)
	Vector3f Vector3f::RotateRad(float Angle) const
	{
		return Mtx4x4::RotRad2D(Angle) * *this;
	}

	//------ Returns a rotated vector by an angle (degrees)
	Vector3f Vector3f::RotateDeg(float Angle) const
	{
		return Mtx4x4::RotDeg2D(Angle) * *this;
	}

	//-------------------------------------------------------------------------------
	// -- Copy assignment operator
	inline Mtx4x4& Mtx4x4::operator=(const Mtx4x4& rhs)
	{
		// -- Copy values
		for (int i = 0; i < MTX_TOTAL_ELEMENTS; ++i)
			m[i] = rhs.m[i];

		// -- Return ref to this
		return *this;
	}

	// -- Operator *
	// -- | Utilises the previously defined operator*=
	inline Mtx4x4 Mtx4x4::operator*(const Mtx4x4& rhs)
	{
		// -- Create matrix result variable
		Mtx4x4 Result(m);

		// -- Compute mtx multiplication via operator*=
		Result *= rhs;

		// -- Return result
		return Result;
	}

	// -- Multiplies a matrix with a vector
	// -- | This operator multiplies the matrix with the rhs vector
	// -- | and returns the result as a vector
	inline Vector3f Mtx4x4::operator*(const Vector3f& rhs)
	{
		// -- Create vector3f result variable
		Vector3f Result;

		// -- Compute matrix vector multiplication
		Result.x = m2[0][0] * rhs.x + m2[0][1] * rhs.y + m2[0][2] * rhs.z + m2[0][3];
		Result.y = m2[1][0] * rhs.x + m2[1][1] * rhs.y + m2[1][2] * rhs.z + m2[1][3];
		Result.z = m2[2][0] * rhs.x + m2[2][1] * rhs.y + m2[2][2] * rhs.z + m2[2][3];

		// -- Return result
		return Result;
	}

	//-------------------------------------------------------------------------------
	// -- Returns the identity matrix
	inline Mtx4x4 Mtx4x4::Identity()
	{
		// -- Construct an identity matrix
		return Mtx4x4(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the translation matrix
	inline Mtx4x4 Mtx4x4::Trans(const Vector3f& vec)
	{
		// -- Construct a translation matrix with ref to the desired vector
		return Mtx4x4(1.f, 0.f, 0.f, vec.x,
			0.f, 1.f, 0.f, vec.y,
			0.f, 0.f, 1.f, vec.z,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the scale matrix
	inline Mtx4x4 Mtx4x4::Scale(const Vector3f& vec)
	{
		// -- Construct a scale matrix with ref to the desired vector
		return Mtx4x4(vec.x, 0.f, 0.f, 0.f,
			0.f, vec.y, 0.f, 0.f,
			0.f, 0.f, vec.z, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the rotation matrix in the X axis
	inline Mtx4x4 Mtx4x4::RotX(float angle)
	{
		// -- Construct a rotation matrix (x axis) with the desired angle
		return Mtx4x4(1.f, 0.f, 0.f, 0.f,
			0.f, MathUtils::Cosf(angle), -MathUtils::Sinf(angle), 0.f,
			0.f, MathUtils::Sinf(angle), MathUtils::Cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the rotation matrix in the Y axis
	inline Mtx4x4 Mtx4x4::RotY(float angle)
	{
		// -- Construct a rotation matrix (y axis) with the desired angle
		return Mtx4x4(MathUtils::Cosf(angle), 0.f, MathUtils::Sinf(angle), 0.f,
			0.f, 1.f, 0.f, 0.f,
			-MathUtils::Sinf(angle), 0.f, MathUtils::Cosf(angle), 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the rotation matrix in the Z axis
	inline Mtx4x4 Mtx4x4::RotZ(float angle)
	{
		// -- Construct a rotation matrix (Z axis) with the desired angle
		return Mtx4x4(MathUtils::Cosf(angle), -MathUtils::Sinf(angle), 0.f, 0.f,
			MathUtils::Sinf(angle), MathUtils::Cosf(angle), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	// -- Returns the rotation matrix (Z axis) taking in an angle in radians
	//inline Mtx4x4 Mtx4x4::RotRad2D(float angle)
	//{
	//	return RotZ(angle);
	//}
	//
	//// -- Returns the rotation matrix (Z axis) taking in an angle in degrees
	//inline Mtx4x4 Mtx4x4::RotDeg2D(float angle)
	//{
	//	return RotZ(MathUtils::DegToRad(angle));
	//}
}
