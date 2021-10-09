#ifndef VECTOR4F_H
#define VECTOR4F_H

#include <stdio.h>

#define ErrorIf(...) ((void)0)

namespace paperback
{
    bool DebugIsZero(float val)
    {
        return std::abs(val) <= 0.00000001f;
    }
    bool IsValid(float val)
    {
#ifdef _MSC_VER
        return _finite(val) != 0;
#else
        return val == val;
#endif
    }
    template <typename T>
    inline T Max(const T lhs, const T rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    template <typename T>
    inline T Min(const T lhs, const T rhs)
    {
        return lhs > rhs ? rhs : lhs;
    }

    struct Vector4f
    {
        Vector4f() {};

        explicit Vector4f(float x_, float y_, float z_, float w_)
        {
            x = x_;
            y = y_;
            z = z_;
            w = w_;
        }
        explicit Vector4f(const float* data)
        {
            array[0] = data[0];
            array[1] = data[1];
            array[2] = data[2];
            array[3] = data[3];
        }
        //Splat all elements
        explicit Vector4f(float xyzw)
        {
            x = y = z = w = xyzw;
        }

        float* ToFloats()
        {
            return (float*)this;
        }

        float& operator[](unsigned index)
        {
            ErrorIf(index > 3, "Math::Vector4 - Subscript out of range.");
            return array[index];
        }
        float operator[](unsigned index) const
        {
            ErrorIf(index > 3, "Math::Vector4 - Subscript out of range.");
            return array[index];
        }

        //Unary Operators
        Vector4f operator-() const
        {
            return Vector4f(-x, -y, -z, -w);
        }

        //Binary Assignment Operators (reals)
        void operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
        }
        void operator/=(float rhs)
        {
            ErrorIf(Math::DebugIsZero(rhs), "Math::Vector4 - Division by zero.");
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
        }

        //Binary Operators (Reals)
        Vector4f operator*(float rhs) const
        {
            return Vector4f(x * rhs, y * rhs, z * rhs, w * rhs);
        }
        Vector4f operator/(float rhs) const
        {
            ErrorIf(Math::DebugIsZero(rhs), "Math::Vector4 - Division by zero.");
            return Vector4f(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        //Binary Assignment Operators (vectors)
        void operator+=(const Vector4f& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
        }
        void operator-=(const Vector4f& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
        }
        void operator*=(const Vector4f& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
        }
        void operator/=(const Vector4f& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
        }

        //Binary Operators (vectors)
        Vector4f operator+(const Vector4f& rhs) const
        {
            return Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }
        Vector4f operator-(const Vector4f& rhs) const
        {
            return Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        //Binary Vector Comparisons
        bool operator==(const Vector4f& rhs) const
        {
            return x == rhs.x &&
                y == rhs.y &&
                z == rhs.z &&
                w == rhs.w;
        }
        bool operator!=(const Vector4f& rhs) const
        {
            return !(*this == rhs);
        }


        //Vector component wise multiply and divide
        Vector4f operator*(const Vector4f& rhs) const
        {
            return Vector4f(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
        }
        Vector4f operator/(const Vector4f& rhs) const
        {
            ErrorIf(rhs.x == 0.0f || rhs.y == 0.0f ||
                rhs.z == 0.0f || rhs.w == 0.0f,
                "Vector4 - Division by Zero.");
            return Vector4f(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
        }
        
        void Set(float x_, float y_, float z_, float w_)
        {
            x = x_;
            y = y_;
            z = z_;
            w = w_;
        }

        ///Set all of the values of the vector to the passed in value.
        void Splat(float xyzw)
        {
            x = y = z = w = xyzw;
        }

        ///Do a component-wise scaling of this vector with the given vector.
        void ScaleByVector(const Vector4f& rhs)
        {
            *this *= rhs;
        }

        Vector4f ScaledByVector(const Vector4f& rhs) const
        {
            return *this * rhs;
        }

        void ZeroOut()
        {
            array[0] = 0.0f;
            array[1] = 0.0f;
            array[2] = 0.0f;
            array[3] = 0.0f;
        }
        void AddScaledVector(const Vector4f& vector, float scalar)
        {
            x += vector.x * scalar;
            y += vector.y * scalar;
            z += vector.z * scalar;
            w += vector.w * scalar;
        }

        float Dot(const Vector4f& rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }
        float Length() const
        {
            return  std::sqrtf(LengthSq());
        }
        float LengthSq() const
        {
            return Dot(*this);
        }
        Vector4f Normalized() const
        {
            Vector4f ret = *this;
            ret /= Length();
            return ret;
        }
        float Normalize()
        {
            float length = Length();
            *this /= length;
            return length;
        }
        float AttemptNormalize()
        {
            float lengthSq = LengthSq();
            if (DebugIsZero(lengthSq) == false)
            {
                lengthSq = std::sqrtf(lengthSq);
                *this /= lengthSq;
            }
            return lengthSq;
        }
        Vector4f& Negate()
        {
            (*this) *= -1.0f;
            return *this;
        }
        bool Valid() const
        {
            return IsValid(x) && IsValid(y) && IsValid(z) && IsValid(w);
        }

        union
        {
            #pragma warning(disable : 4201)
            struct
            {
                float x, y, z, w;
            };
            #pragma warning(default : 4201)
            float array[4];
        };

        static const Vector4f cZero;
        static const Vector4f cXAxis;
        static const Vector4f cYAxis;
        static const Vector4f cZAxis;
        static const Vector4f cWAxis;
    };

    Vector4f operator*(float lhs, const Vector4f& rhs)
    {
        return rhs * lhs;
    }
    float Dot(const Vector4f& lhs, const Vector4f& rhs)
    {
        return lhs.Dot(rhs);
    }
    float Length(const Vector4f& vect)
    {
        return vect.Length();
    }
    float LengthSq(const Vector4f& vect)
    {
        return vect.LengthSq();
    }
    Vector4f Normalized(const Vector4f& vect)
    {
        return vect.Normalized();
    }
    float Normalize(Vector4f* vect = nullptr)
    {
        ErrorIf(vect == NULL, "Vector4 - Null pointer passed for vector.");
        return vect->Normalize();
    }
    float AttemptNormalize(Vector4f* vect)
    {
        ErrorIf(vect == NULL, "Vector4 - Null pointer passed for vector.");
        return vect->AttemptNormalize();
    }
    void Negate(Vector4f* vec)
    {
        ErrorIf(vec == NULL, "Vector4 - Null pointer passed for vector.");
        *vec *= -1.0f;
    }
    Vector4f Negated(const Vector4f& vec)
    {
        return Vector4f{ -vec.x, -vec.y, -vec.z, -vec.w };
    }
    Vector4f Abs(const Vector4f& vec)
    {
        return Vector4f(std::abs(vec.x), std::abs(vec.y),
            std::abs(vec.z), std::abs(vec.w));
    }
    Vector4f Min(const Vector4f& lhs, const Vector4f& rhs)
    {
        return Vector4f(Min(lhs.x, rhs.x),
            Min(lhs.y, rhs.y),
            Min(lhs.z, rhs.z),
            Min(lhs.w, rhs.w));
    }
    Vector4f Max(const Vector4f& lhs, const Vector4f& rhs)
    {
        return Vector4f(Max(lhs.x, rhs.x),
            Max(lhs.y, rhs.y),
            Max(lhs.z, rhs.z),
            Max(lhs.w, rhs.w));
    }
    Vector4f Lerp(const Vector4f& start, const Vector4f& end, float tValue)
    {
        return Vector4f(start[0] + tValue * (end[0] - start[0]),
            start[1] + tValue * (end[1] - start[1]),
            start[2] + tValue * (end[2] - start[2]),
            start[3] + tValue * (end[3] - start[3]));
    }

    const Vector4f Vector4f::cZero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4f Vector4f::cXAxis(1.0f, 0.0f, 0.0f, 0.0f);
    const Vector4f Vector4f::cYAxis(0.0f, 1.0f, 0.0f, 0.0f);
    const Vector4f Vector4f::cZAxis(0.0f, 0.0f, 1.0f, 0.0f);
    const Vector4f Vector4f::cWAxis(0.0f, 0.0f, 0.0f, 1.0f);
}

//#include "Vector3f.hpp"

#endif
