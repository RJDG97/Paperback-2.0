#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector3fData.h"

namespace paperback
{
	//------ A Vector3f object can be constructed with only 2 floats
	//------| E.g. Vector3f Vec(1.f, 1.f);
	//------| and the Z component will be defaulted to 0. This means that
	//------| the Vector3f class can be utilised for 2D environments as well.
	class Vector3f
	{
	public:															//------ Public members
		float x, y, z;												//------ x, y, z members
		Vector3f(float _x, float _y, float _z)						//------ Overloaded constructor
			: x(_x), y(_y), z(_z)
		{}
		Vector3f(float _x, float _y)								//------ Overloaded constructor [2D]
			: Vector3f(_x, _y, 0.f)
		{}
		Vector3f()													//------ Default constructor
			: Vector3f(0.f, 0.f, 0.f)
		{}
		inline void Set(float _x, float _y, float _z);				//------ Sets all components to _x, _y, _z
		inline void Set(const Vector3f& Other);						//------ Sets all components to Vector3f param via operator=
		//----------------------------------------------------------------------//
		
		inline float MagnitudeSq() const;							//------ Returns the squared magnitude 
		inline float Magnitude() const;								//------ Returns the magnitude
		inline void Normalize();									//------ Normalizes the vector
		inline Vector3f Normalized() const;							//------ Returns a normalized vector copy (does not modify *this)
		inline void Reset();										//------ Resets all variables to 0
		inline float Dot(const Vector3f& Other) const;				//------ Returns the dot product of the 2 vectors
		inline Vector3f Cross(const Vector3f& Other) const;			//------ Returns the cross product of the 2 vectors
		float Angle(const Vector3f& Other) const					//------ Returns the angle between the 2 vectors
		{
			// -- Get magnitude multipication of both vectors
			float f_MagMul = Magnitude() * Other.Magnitude();

			// -- Divide by zero check
			if (f_MagMul == 0)
				return 0.f;

			// -- Return angle 
			return acosf(Dot(Other) / f_MagMul);
		}
		inline bool IsZero(void) const;								//------ Checks if a vector is zero
		inline void Negate();										//------ Negates the vector via operator-

		//----------------------------------------------------------------------//
		inline Vector3f operator-() const;							//------ Negates all components
		inline Vector3f& operator*=(float Scalar);					//------ Vector3f *= Scalar
		inline Vector3f operator*(float Scalar) const;				//------ Vector3f * Scalar
		inline Vector3f& operator/=(float Scalar);					//------ Vector3f /= Scalar
		inline Vector3f operator/(float Scalar) const;				//------ Vector3f / Scalar
		inline Vector3f& operator+=(const Vector3f& Other);			//------ Vector3f += Vector3f
		inline Vector3f operator+(const Vector3f& Other) const;		//------ Vector3f + Vector3f
		inline Vector3f& operator-=(const Vector3f& Other);			//------ Vector3f -= Vector3f
		inline Vector3f operator-(const Vector3f& Other) const;		//------ Vector3f - Vector3f
		inline bool operator==(const Vector3f& Other) const;		//------ Vector3f == Vector3f
		inline bool operator!= (const Vector3f& Other) const;		//------ Vector3f != Vector3f
		inline Vector3f operator~() const;							//------ Returns 2D-casted Vector3f
		//----------------------------------------------------------------------//

		float MagnitudeFast() const;								//------ Find approximated magnitude of a vector using fast inverse sqrt
		inline Vector3f NormalizedFast() const;						//------ Normalizing of a vector with fast inverse sqrt
		inline Vector3f ScaleToLength(float Salar) const;			//------ Returns a scaled vector of desired length
		inline Vector3f Reflect(const Vector3f& Normal) const;		//------ Reflects a vector about a surface
		inline Vector3f Normal2D() const;							//------ Gets the 2D normal vector
		Vector3f RotateRad(float Angle) const;						//------ Returns a rotated vector by an angle (radians)
		//{
		//	return Mtx4x4::RotRad2D(Angle) * *this;
		//}
		Vector3f RotateDeg(float Angle) const;						//------ Returns a rotated vector by an angle (degrees)
		//{
		//	return Mtx4x4::RotDeg2D(Angle) * *this;
		//}
		inline Vector3f RotateAboutPoint(const Vector3f& Point,		//------ Rotates a point about a 
			const Vector3f& Center,	//------ desired centre point with
			float Degrees);			//------ a desired angle.
	};

	//------ Operator overload | operator* | Scalar * Vector3f
	Vector3f operator*(float lhs, const Vector3f& rhs);

	//------ Operator overload | operator/ | Scalar / Vector3f
	Vector3f operator/(float lhs, const Vector3f& rhs);
}

#include "Vector3f.hpp"

#endif
