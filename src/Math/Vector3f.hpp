#include "MathUtils.h"

namespace paperback
{
	//----- Operator overload  
	//-----| unary operator-  
	//-----| Negates all components
	inline Vector3f Vector3f::operator-() const { 
		return Vector3f(-x, -y, -z); 
	}

	//----- Operator overload  
	//-----| operator*=  
	//-----| Vector3f *= scalar
	inline Vector3f& Vector3f::operator*=(float Scalar) {
		x *= Scalar;
		y *= Scalar;
		z *= Scalar;
		return *this;
	}

	//----- Operator overload  
	//-----| operator*  
	//-----| Vector3f * scalar
	inline Vector3f Vector3f::operator*(float Scalar) const {
		Vector3f Temp = *this;
		Temp *= Scalar;
		return Temp;
	}

	//----- Operator overload  
	//-----| operator/=  
	//-----| Vector3f /= scalar
	inline Vector3f& Vector3f::operator/=(float Scalar) {
		x /= Scalar;
		y /= Scalar;
		z /= Scalar;
		return *this;
	}

	// -- Operator overload  
	// --| operator/  
	// --| Vector3f / scalar
	inline Vector3f Vector3f::operator/(float Scalar) const {
		Vector3f Temp = *this;
		Temp /= Scalar;
		return Temp;
	}

	// -- Operator overload  
	// --| operator+=  
	// --| Vector3f += Vector3f
	inline Vector3f& Vector3f::operator+=(const Vector3f& Other) {
		x += Other.x;
		y += Other.y;
		z += Other.z;
		return *this;
	}

	// -- Operator overload  
	// --| operator+  
	// --| Vector3f + Vector3f
	inline Vector3f Vector3f::operator+(const Vector3f& Other) const {
		Vector3f Temp = *this;
		Temp += Other;
		return Temp;
	}

	// -- Operator overload  
	// --| operator-=  
	// --| Vector3f -= Vector3f
	inline Vector3f& Vector3f::operator-=(const Vector3f& Other) {
		x -= Other.x;
		y -= Other.y;
		z -= Other.z;
		return *this;
	}

	// -- Operator overload  
	// --| operator-  
	// --| Vector3f - Vector3f
	inline Vector3f Vector3f::operator-(const Vector3f& Other) const {
		Vector3f Temp = *this;
		Temp -= Other;
		return Temp;
	}

	// -- Operator overload  
	// --| operator==  
	// --| Vector3f == Vector3f
	inline bool Vector3f::operator==(const Vector3f& Other) const {
		// -- Compare floats with IsEqualf (EPSILON comparison)
		return MathUtils::IsEqualf(x, Other.x) &&
			MathUtils::IsEqualf(y, Other.y) &&
			MathUtils::IsEqualf(z, Other.z);
	}

	// -- Operator overload  
	// -- | operator!=  
	// -- | Vector3f != Vector3f
	inline bool Vector3f::operator!=(const Vector3f& Other) const { 
		return !(*this == Other); 
	}

	// -- Operator overload  
	// --| operator*  
	// --| Scalar * Vector3f
	inline Vector3f operator*(float lhs, const Vector3f& rhs) { 
		return rhs * lhs; 
	}

	// -- Operator overload  
	// --| operator/  
	// --| Scalar / Vector3f
	inline Vector3f operator/(float lhs, const Vector3f& rhs) { 
		return rhs / lhs; 
	}

	// -- Operator overload  
	// --| unary operator!
	// --| Returns the 2D-casted Vector3f
	// --| with the z-component set to 0.f.
	inline Vector3f Vector3f::operator~() const { 
		return Vector3f(x, y, 0.f); 
	}

	// -- IsZero function  
	// --| Checks if a vector is a zero vector
	inline bool Vector3f::IsZero(void) const { 
		return *this == Vector3f(); 
	}

	// -- Negate function  
	// --| Negate all values of this vector
	inline void Vector3f::Negate() { 
		*this = -(*this); 
	}

	// -- Vector3f set function
	inline void Vector3f::Set(float _x, float _y, float _z) { 
		*this = Vector3f(_x, _y, _z); 
	}

	// -- Sets all components to Vector3f param via operator=
	inline void Vector3f::Set(const Vector3f& Other) { 
		*this = Other; 
	}

	// -- MagnitudeSquared function 
	// -- | Formula: x * x + y * y + z * z
	inline float Vector3f::MagnitudeSq() const { 
		return x * x + y * y + z * z; 
	}

	// -- Magnitude function 
	// -- | Formula: sqrt(x * x + y * y + z * z) 
	inline float Vector3f::Magnitude() const { 
		return sqrtf(MagnitudeSq()); 
	}

	// -- Normalize function  
	// -- | Formula: x / Magnitude, y / Magnitude, z / Magnitude
	inline void Vector3f::Normalize() { 
		*this /= Magnitude(); 
	}

	// -- Normalized function  
	// -- | Returns a normalized vector copy (does not modify *this)
	inline Vector3f Vector3f::Normalized() const 
	{
		Vector3f Temp = *this;
		Temp.Normalize();
		return Temp;
	}

	// -- Reset function  
	// -- | Resets all variables to 0
	inline void Vector3f::Reset() { 
		*this = Vector3f(); 
	}

	// -- Dot Product function  
	// -- | Formula: x * Other.x + y * Other.y + z * Other.z
	inline float Vector3f::Dot(const Vector3f& Other) const	{
		return Other.x * x + Other.y * y + Other.z * z;
	}

	// -- Cross Product function  
	// -- | Formula: y1z2 - z1y2, z1x2 - x1z2, x1y2 - y1x2
	inline Vector3f Vector3f::Cross(const Vector3f& Other) const 
	{
		return Vector3f(y * Other.z - z * Other.y,
			z * Other.x - x * Other.z,
			x * Other.y - y * Other.x);
	}

	// -- Find approx magnitude of a vector using inverse sqrt
	// -- | Note: approximation, does not provide an accurate result
	inline float Vector3f::MagnitudeFast() const {
		return 1 / MathUtils::Inv_sqrt(Magnitude());
	}

	// -- Normalizing of a vector with inverse sqrt
	inline Vector3f Vector3f::NormalizedFast() const {
		return *this * MathUtils::Inv_sqrt(MagnitudeSq());
	}

	// -- Returns a scaled vector of desired length
	inline Vector3f Vector3f::ScaleToLength(float Scalar) const {
		return this->NormalizedFast() * Scalar;
	}

	// -- Reflects a vector about a surface
	inline Vector3f Vector3f::Reflect(const Vector3f& Normal) const 
	{
		Vector3f NormalizedNormal = Normal.NormalizedFast();
		return *this - NormalizedNormal * (this->Dot(NormalizedNormal) * 2.f);
	}

	// -- Returns the 2D normal vector
	inline Vector3f Vector3f::Normal2D() const {
		return Vector3f(-y, x);
	}

	// -- Rotates a point about a desired center
	inline Vector3f Vector3f::RotateAboutPoint(const Vector3f& Point, const Vector3f& Center, float Degrees) {
		return Vector3f(Point - Center).RotateDeg(Degrees) + Center;
	}

	inline Vector3f& Vector3f::LockingValue(const Vector3f& lock) {
		if (abs(x) > lock.x)
			x = (x > EPSILON) ? lock.x : -lock.x;
		if (abs(y) > lock.y)
			y = (y > EPSILON) ? lock.y : -lock.y;
		if (abs(z) > lock.z)
			z = (z > EPSILON) ? lock.z : -lock.z;
		return *this;
	}
	inline Vector3f& Vector3f::CutoffValue(const Vector3f& lock) {
		if (abs(x) < lock.x)
			x = 0.f;
		if (abs(y) < lock.y)
			y = 0.f;
		if (abs(z) < lock.z)
			z = 0.f;
		return *this;
	}

	inline Vector3f& Vector3f::DecrementValue(float lock) {
		if (abs(x) > EPSILON)
			x -= (x > EPSILON) ? lock : -lock;
		if (abs(y) > EPSILON)
			y -= (y > EPSILON) ? lock : -lock;
		if (abs(z) > EPSILON)
			z -= (z > EPSILON) ? lock : -lock;
		return *this;
	}
}
