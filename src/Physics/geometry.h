// ----- Start of header guard
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Math/Vector3f.h"
#include "Math/Vector4f.h"
#include "Shapes/Plane.h"



namespace IntersectionType
{
    enum Type { Coplanar = 0, Outside, Inside, Overlaps, NotImplemented };
    static const char* Names[] = { "Coplanar", "Outside", "Inside", "Overlaps", "NotImplemented" };
};

paperback::Vector3f ProjectPointOnPlane(const paperback::Vector3f& point, const paperback::Vector3f& normal, float planeDistance)
{
    //ok
    float dist_pntPlane = point.Dot(normal) - planeDistance;
    return point - dist_pntPlane * normal;
}

bool BarycentricCoordinates(const paperback::Vector3f& point, const paperback::Vector3f& a, const paperback::Vector3f& b,
    float& u, float& v, float epsilon)
{
    //ok
      // checks for zero division
    if (a == b)
    {
        u = v = 0.f;
        return false;
    }

    paperback::Vector3f ab = a - b;
    u = (point - b).Dot(ab) / ab.Dot(ab);
    v = 1.f - u;

    if (u <= -epsilon || u >= 1.f + epsilon)
        return false;
    return true;
}

bool BarycentricCoordinates(const paperback::Vector3f& point, const paperback::Vector3f& a, const paperback::Vector3f& b, const paperback::Vector3f& c,
    float& u, float& v, float& w, float epsilon)
{
    //1
    paperback::Vector3f v0 = point - c, v1 = a - c, v2 = b - c;
    paperback::Vector3f denom = v1.Cross(v2);
    // check denominator is 0.f
    if (denom.MagnitudeSq() <= 0.f)
    {
        u = v = w = 0.f;
        return false;
    }
    // determinant
    float E = v0.Dot(v1);
    float F = v0.Dot(v2);
    float A = v1.Dot(v1);
    float B = v2.Dot(v1);
    float C = B;
    float D = v2.Dot(v2);
    float bot = A * D - B * C;

    if (bot >= -epsilon && bot < epsilon)
        return false;
    u = (E * D - B * F) / bot;
    v = (A * F - E * C) / bot;
    w = 1.f - u - v;
    float check = u + v + w;
    // rejection case
    if ((u < -epsilon) || (v < -epsilon) || (w < -epsilon))
        return false;
    if ((u > 1.f + epsilon) || (v > 1.f + epsilon) || (w > 1.f + epsilon))
        return false;
    // by this point, it should return true.
    if (check >= 1.f - epsilon && check <= 1.f + epsilon)
        return true;
    return false;
}

IntersectionType::Type PointPlane(const paperback::Vector3f& point, const paperback::Vector4f& plane, float epsilon)
{
    //ok
    float w = paperback::Vector4f(point.x, point.y, point.z, -1.f).Dot(plane);
    if (w > epsilon)
        return IntersectionType::Inside;
    else
    {
        if (w < -epsilon)
            return IntersectionType::Outside;
        else
            return IntersectionType::Coplanar;
    }
}

bool PointSphere(const paperback::Vector3f& point, const paperback::Vector3f& sphereCenter, float sphereRadius)
{
    //ok
    float length_sq = (point - sphereCenter).MagnitudeSq();
    return length_sq <= sphereRadius * sphereRadius;
}

bool PointAabb(const paperback::Vector3f& point, const paperback::Vector3f& aabbMin, const paperback::Vector3f& aabbMax)
{
    //ok
    return point.x >= aabbMin.x && point.x <= aabbMax.x
        && point.y >= aabbMin.y && point.y <= aabbMax.y
        && point.z >= aabbMin.z && point.z <= aabbMax.z;
}

bool RayPlane(const paperback::Vector3f& rayStart, const paperback::Vector3f& rayDir,
    const paperback::Vector4f& plane, float& t, float epsilon)
{
    //1
    paperback::Vector3f n = paperback::Vector3f(plane.x, plane.y, plane.z);
    float nStart = n.Dot(rayStart);
    float nDir = n.Dot(rayDir);

    if (nDir <= epsilon && nDir >= -epsilon) return false;

    t = (plane.w - nStart) / nDir;
    if (t >= epsilon)
        return true;
    return false;
}

bool RayTriangle(const paperback::Vector3f& rayStart, const paperback::Vector3f& rayDir,
    const paperback::Vector3f& triP0, const paperback::Vector3f& triP1, const paperback::Vector3f& triP2,
    float& t, float triExpansionEpsilon)
{
    //ok
    Plane plane(triP0, triP1, triP2);
    if (RayPlane(rayStart, rayDir, plane.mData, t, triExpansionEpsilon))
    {
        paperback::Vector3f point = rayStart + rayDir * t;
        float u = 0.f, v = 0.f, w = 0.f;
        return BarycentricCoordinates(point, triP0, triP1, triP2, u, v, w, triExpansionEpsilon);
    }
    return false;
}

bool RaySphere(const paperback::Vector3f& rayStart, const paperback::Vector3f& rayDir,
    const paperback::Vector3f& sphereCenter, float sphereRadius,
    float& t)
{
    //-
    if (PointSphere(rayStart, sphereCenter, sphereRadius))
    {
        t = 0.f;
        return true;
    }
    float a = rayDir.Dot(rayDir);
    float b = 2.f * (rayStart.Dot(rayDir) - sphereCenter.Dot(rayDir));
    float c = sphereCenter.Dot(sphereCenter) + rayStart.Dot(rayStart)
        - 2.f * sphereCenter.Dot(rayStart) - sphereRadius * sphereRadius;
    float det = b * b - 4.f * a * c;

    if (det >= 0.f)
    {
        if (det == 0.f)
        {
            t = (-b + sqrtf(det)) / (2.f * a);
            return t > 0.f;
        }
        else
        {
            float t0 = (-b + sqrtf(det)) / (2.f * a);
            float t1 = (-b - sqrtf(det)) / (2.f * a);

            if (t0 < t1)
                t = t0;
            else
                t = t1;
            return t > 0.f;
        }
    }
    else
    {
        t = -1.0f;
        return false;
    }
}

bool RayAabb(const paperback::Vector3f& rayStart, const paperback::Vector3f& rayDir,
    const paperback::Vector3f& aabbMin, const paperback::Vector3f& aabbMax, float& t)
{
    //1
    float t_max = FLT_MAX;
    paperback::Vector3f n = rayDir.Normalized();
    //int dirCheck = 0;
    // checks all the axis
    for (size_t i = 0; i < 3; ++i)
    {
        // check direction of ray
        if (n[static_cast<unsigned>(i)] == 0.f)
        {
            if (rayStart[static_cast<unsigned>(i)] > aabbMax[static_cast<unsigned>(i)] || rayStart[static_cast<unsigned>(i)] < aabbMin[static_cast<unsigned>(i)])
                return false;
            continue;
        }

        float max = (aabbMax[static_cast<unsigned>(i)] - rayStart[static_cast<unsigned>(i)]) / n[static_cast<unsigned>(i)];
        float min = (aabbMin[static_cast<unsigned>(i)] - rayStart[static_cast<unsigned>(i)]) / n[static_cast<unsigned>(i)];

        if (max < min)
            std::swap(max, min);

        if (t > max || t_max < min)
            return false;

        t = std::max(t, min);
        t_max = std::min(t_max, max);

        if (t > t_max)
            return false;
    }
    return true;
}

IntersectionType::Type PlaneTriangle(const paperback::Vector4f& plane,
    const paperback::Vector3f& triP0, const paperback::Vector3f& triP1, const paperback::Vector3f& triP2,
    float epsilon)
{
    //-
    char collide = 0;

    IntersectionType::Type type = PointPlane(triP0, plane, epsilon);
    if (type == IntersectionType::Coplanar)
        collide |= 0x1;
    else if (type == IntersectionType::Inside)
        collide |= 0x2;
    else //(type == IntersectionType::Outside) 
        collide |= 0x4;

    type = PointPlane(triP1, plane, epsilon);
    if (type == IntersectionType::Coplanar)
        collide |= 0x1;
    else if (type == IntersectionType::Inside)
        collide |= 0x2;
    else //(type == IntersectionType::Outside) 
        collide |= 0x4;

    type = PointPlane(triP2, plane, epsilon);
    if (type == IntersectionType::Coplanar)
        collide |= 0x1;
    else if (type == IntersectionType::Inside)
        collide |= 0x2;
    else //(type == IntersectionType::Outside) 
        collide |= 0x4;

    if (collide == 0x1)
        return IntersectionType::Coplanar;
    else if (collide == 0x2 || collide == 0x3)
        return IntersectionType::Inside;
    else if (collide == 0x4 || collide == 0x5)
        return IntersectionType::Outside;
    else
        return IntersectionType::Overlaps;
}

IntersectionType::Type PlaneSphere(const paperback::Vector4f& plane,
    const paperback::Vector3f& sphereCenter, float sphereRadius)
{
    //-
    // diff norm
    IntersectionType::Type type = PointPlane(sphereCenter, plane, sphereRadius);
    if (type == IntersectionType::Coplanar)
        return IntersectionType::Overlaps;
    return type;
}

IntersectionType::Type PlaneAabb(const paperback::Vector4f& plane,
    const paperback::Vector3f& aabbMin, const paperback::Vector3f& aabbMax)
{
    //1
    paperback::Vector3f n(plane.x, plane.y, plane.z);
    paperback::Vector3f point_c = (aabbMax + aabbMin) * 0.5f;
    paperback::Vector3f e = aabbMax - point_c;

    float rad = e.x * abs(n.x) + e.y * abs(n.y) + e.z * abs(n.z);

    return PlaneSphere(plane, point_c, rad);
}

IntersectionType::Type FrustumTriangle(const paperback::Vector4f planes[6],
    const paperback::Vector3f& triP0, const paperback::Vector3f& triP1, const paperback::Vector3f& triP2,
    float epsilon)
{
    //-             //ok
    char collide = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        IntersectionType::Type type = PlaneTriangle(planes[i], triP0, triP1, triP2, epsilon);

        if (type == IntersectionType::Outside)
            return IntersectionType::Outside;
        else if (type == IntersectionType::Overlaps)
            collide |= 0x1;
        else //(type == IntersectionType::Inside) 
            collide |= 0x2;
    }
    if (collide == 0x2)
        return IntersectionType::Inside;
    else
        return IntersectionType::Overlaps;
}

IntersectionType::Type FrustumSphere(const paperback::Vector4f planes[6],
    const paperback::Vector3f& sphereCenter, float sphereRadius, size_t& lastAxis)
{
    //-            //ok
    char collide = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        IntersectionType::Type type = PlaneSphere(planes[i], sphereCenter, sphereRadius);

        if (type == IntersectionType::Outside)
        {
            lastAxis = i;
            return IntersectionType::Outside;
        }

        if (type == IntersectionType::Overlaps)
            collide |= 0x1;
        else //(type == IntersectionType::Inside) 
            collide |= 0x2;
    }
    if (collide == 0x2)
        return IntersectionType::Inside;
    else
        return IntersectionType::Overlaps;
}

IntersectionType::Type FrustumAabb(const paperback::Vector4f planes[6],
    const paperback::Vector3f& aabbMin, const paperback::Vector3f& aabbMax, size_t& lastAxis)
{
    //-1
    char collide = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        size_t c = (lastAxis + i) % 6;
        IntersectionType::Type type = PlaneAabb(planes[c], aabbMin, aabbMax);

        if (type == IntersectionType::Outside)
        {
            lastAxis = c;
            return IntersectionType::Outside;
        }

        if (type == IntersectionType::Overlaps)
            collide |= 0x1;
        else //(type == IntersectionType::Inside) 
            collide |= 0x2;
    }
    if (collide == 0x2)
        return IntersectionType::Inside;
    else
        return IntersectionType::Overlaps;
}

bool SphereSphere(const paperback::Vector3f& sphereCenter0, float sphereRadius0,
    const paperback::Vector3f& sphereCenter1, float sphereRadius1)
{
    //ok
    float length_sq = (sphereCenter0 - sphereCenter1).MagnitudeSq();
    float radius = sphereRadius0 + sphereRadius1;
    return length_sq <= radius * radius;
}

bool AabbAabb(const paperback::Vector3f& aabbMin0, const paperback::Vector3f& aabbMax0,
    const paperback::Vector3f& aabbMin1, const paperback::Vector3f& aabbMax1)
{
    //ok
    return !(aabbMin0.x > aabbMax1.x || aabbMin1.x > aabbMax0.x
        || aabbMin0.y > aabbMax1.y || aabbMin1.y > aabbMax0.y
        || aabbMin0.z > aabbMax1.z || aabbMin1.z > aabbMax0.z);
}


#endif