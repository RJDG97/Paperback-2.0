#ifndef QUATERNION_H
#define QUATERNION_H

#include <stdio.h>
#include "Vector3f.h"
#include "Vector4f.h"
#include "MathUtils.h"

#define ErrorIf(...) ((void)0)


namespace paperback
{
	// b_rot is degrees -> convert to radians 1st (do all in degrees if possible)
	

	// b_rot is degrees -> Use Mat4x4 -> compose z * y * x -> create mat4x4 rotation angle -> Euler angles
	

	// Quat
	// w^2 -  x^2 - y^2 - z^2 = -1
	//  x^2 = y^2 = z^2 = 1
	// 2xw = 2yw = 2yz = 0

	// given vec3f = x , y , z

	// convert quaternion -> angle of rotation -> 
	struct Quaternion
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float array[4];
		};

		Quaternion()
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 1.f;
		}
		explicit Quaternion(float x_, float y_, float z_, float w_)
		{
			x = x_;
			y = y_;
			z = z_;
			w = w_;
		}

		void operator=(Quaternion rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
		}
		void operator+=(Quaternion rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
		}
		void operator-=(Quaternion rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
		}
		void operator*=(Quaternion rhs)
		{
			Quaternion lhs(x, y, z, w);
			x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
			y = lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
			z = lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
			w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
		}
		void operator*=(float rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
		}
		void operator/=(float rhs)
		{
			if (rhs != 0.f)
			{
				float reciprocal = 1.0f / rhs;
				x *= reciprocal;
				y *= reciprocal;
				z *= reciprocal;
				w *= reciprocal;
			}
		}

		Quaternion operator-() const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		Quaternion operator*(Quaternion rhs) const
		{
			return Quaternion(
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
				w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
		}
		Quaternion operator+(Quaternion rhs) const
		{
			return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}
		Quaternion operator-(Quaternion rhs) const
		{
			return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}
		Quaternion operator*(float rhs) const
		{
			return Quaternion(x * rhs, y * rhs, z * rhs, w * rhs);
		}
		Quaternion operator/(float rhs) const
		{
			if (rhs != 0.f)
			{
				return Quaternion(x / rhs, y / rhs, z / rhs, w / rhs);
			}
		}

		bool operator==(Quaternion rhs) const
		{
			return 
				x == rhs.x &&
				y == rhs.y &&
				z == rhs.z &&
				w == rhs.w;
		}
		bool operator!=(Quaternion rhs) const
		{
			return !(*this == rhs);
		}

		float& operator[](unsigned index)
		{
			ErrorIf(index > 3, "Quaternion - Subscript out of range.");
			return array[index];
		}
		float operator[](unsigned index) const
		{
			ErrorIf(index > 3, "Quaternion - Subscript out of range.");
			return array[index];
		}
		void Set(float x_, float y_, float z_, float w_)
		{
			x = x_;
			y = y_;
			z = z_;
			w = w_;
		}

		void Integrate(Vector3f vector, float scalar)
		{
			Quaternion q(vector.x * scalar, vector.y * scalar, vector.z * scalar, 0.0f);

			q *= *this;
			x += 0.5f * q.x;
			y += 0.5f * q.y;
			z += 0.5f * q.z;
			w += 0.5f * q.w;
		}

		float Normalize()
		{
			float length = LengthSq();

			if (length == 0.0f)
			{
				x = 0.0f;
				y = x;
				z = x;
				w = 1.0f;
				return 0.0f;
			}
			else
			{
				length = 1.0f / std::sqrt(length);
				*this *= length;
				return length;
			}
		}
		Quaternion Normalized() const
		{
			float length = LengthSq();

			if (length == 0.0f)
			{
				return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				length = 1.0f / std::sqrt(length);
				return (*this) * length;
			}
		}
		float Dot(Quaternion rhs) const
		{
			return V4().Dot(rhs.V4());
		}
		float Length() const
		{
			return std::sqrt(LengthSq());
		}
		float LengthSq() const
		{
			return V4().LengthSq();
		}

		void Conjugate()
		{
			x *= -1.f;
			y *= -1.f;
			z *= -1.f;
		}
		Quaternion Conjugated() const
		{
			return Quaternion(-x, -y, -z, w);
		}
		void Invert()
		{
			Conjugate();
			float lengthSq = LengthSq();
			if (lengthSq != 0.f)
			{
				*this /= lengthSq;
			}
		}
		Quaternion Inverted() const
		{
			float lengthSq = LengthSq();
			if (lengthSq != 0.f)
			{
				return Quaternion(-x / lengthSq, -y / lengthSq, -z / lengthSq, w / lengthSq);
			}
		}
		//
		//

		void ToQuaternion(Quaternion& quaternion, const Vector3f& vector)
		{
			quaternion.Set(vector.x, vector.y, vector.z, 0.0f);
		}

		Quaternion ToQuaternion(const Vector3f& vector)
		{
			Quaternion quaternion;
			ToQuaternion(quaternion, vector);
			return quaternion;
		}
		// z , y , x
		Quaternion ToQuaternion(float yaw, float pitch, float roll)
		{
			float cy = cosf(yaw * .5f);
			float sy = sinf(yaw * .5f);
			float cp = cosf(pitch * .5f);
			float sp = sinf(pitch * .5f);
			float cr = cosf(roll * .5f);
			float sr = sinf(roll * .5f);

			Quaternion q;
			q.w = cr * cp * cy + sr * sp * sy;
			q.x = sr * cp * cy - cr * sp * sy;
			q.y = cr * sp * cy + sr * cp * sy;
			q.z = cr * cp * sy - sr * sp * cy;

			return q;
		}

		Vector3f ToEulerAngles(Quaternion q)
		{
			Vector3f angles;

			// (x-axis rotation)
			float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			angles.x = std::atan2(sinr_cosp, cosr_cosp);

			// (y-axis rotation)
			float sinp = 2 * (q.w * q.y - q.z * q.x);
			if (std::abs(sinp) >= 1)
				angles.y = std::copysign(PIf / 2, sinp); // use 90 degrees if out of range
			else
				angles.y = std::asin(sinp);

			// (z-axis rotation)
			float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			angles.z = std::atan2(siny_cosp, cosy_cosp);

			return angles;
		}

		Vector3f RotatedVector(const Vector3f& vector)
		{
			ToQuaternion(vector);
			Quaternion tempVec = ToQuaternion(vector);
			Quaternion conjugate(-x, -y, -z, w);
			Quaternion result(*this);
			result *= tempVec;
			result *= conjugate;
			return Vector3f(result.x, result.y, result.z);
		}

		void RotateVector(Vector3f& vector)
		{
			Quaternion tempVec = ToQuaternion(vector);
			Quaternion conjugate(-x, -y, -z, w);
			Quaternion result(*this);
			result *= tempVec;
			result *= conjugate;
			vector.Set(result.x, result.y, result.z);
		}
		//

		bool Valid() const
		{
			return IsValid(x) && IsValid(y) &&
				IsValid(z) && IsValid(w);
		}

		Vector3f& V3()
		{
			return *(Vector3f*)this;
		}
		Vector4f& V4()
		{
			return *(Vector4f*)this;
		}
		const Vector3f& V3() const
		{
			return *(Vector3f*)this;
		}
		const Vector4f& V4() const
		{
			return *(Vector4f*)this;
		}
	};
}
/*
*/
#endif
