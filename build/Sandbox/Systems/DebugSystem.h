#pragma once
#include "Components/Transform.h"
#include "../Functionality/Renderer/Renderer.h"
#include "Math/Vector3f.h"

struct debug_system : paperback::system::instance
{

    const size_t m_NumSystems = 15;

    std::array<std::vector<glm::vec3>, 2> m_Points;

    bool m_IsDebug;


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "debug_system"
    };

    //helper function to convert Vector3f to glm::vec3
    glm::vec3 ConvertMathVecToGLMVec(const paperback::Vector3f& Vec3)
    {

        return { Vec3.x, Vec3.y, Vec3.z };
    }

    // given a set of points definining a circle, add to a vector the necessary connections to draw
    void ConvertVerticesToCircleDraw(std::vector<paperback::Vector3f>& Vec,
        const paperback::Vector3f& Top,
        const paperback::Vector3f& Bottom,
        const paperback::Vector3f& Right,
        const paperback::Vector3f& Left,
        const paperback::Vector3f& Top_right,
        const paperback::Vector3f& Bottom_right,
        const paperback::Vector3f& Top_left,
        const paperback::Vector3f& Bottom_left)
    {

        Vec.push_back(Top);
        Vec.push_back(Top_right);

        Vec.push_back(Top_right);
        Vec.push_back(Right);

        Vec.push_back(Right);
        Vec.push_back(Bottom_right);

        Vec.push_back(Bottom_right);
        Vec.push_back(Bottom);

        Vec.push_back(Bottom);
        Vec.push_back(Bottom_left);

        Vec.push_back(Bottom_left);
        Vec.push_back(Left);

        Vec.push_back(Left);
        Vec.push_back(Top_left);

        Vec.push_back(Top_left);
        Vec.push_back(Top);
    }



    // draws a sphere given the provided data
    // low poly circle
    void DrawSphereCollision(sphere& Sphere, transform& Transform)
    {

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f center = Transform.m_Position + Transform.m_Offset;
        paperback::Vector3f top = center;
        paperback::Vector3f bottom = top;

        float radius = Sphere.m_fRadius;

        top.y += radius;
        bottom.y -= radius;

        //matrix rotation not working currently, temp solution
        //circle for default front facing
        paperback::Vector3f top_right = center + paperback::Vector3f{ .707f * radius, .707f * radius, 0.0f };
        paperback::Vector3f top_left = center + paperback::Vector3f{ -.707f * radius, .707f * radius, 0.0f };
        paperback::Vector3f right = center + paperback::Vector3f{ radius, 0.0f, 0.0f };
        paperback::Vector3f left = center + paperback::Vector3f{ -radius, 0.0f, 0.0f };
        paperback::Vector3f bottom_right = center + paperback::Vector3f{ .707f * radius, -.707f * radius, 0.0f };
        paperback::Vector3f bottom_left = center + paperback::Vector3f{ -.707f * radius, -.707f * radius, 0.0f };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //circle for side facing
        top_right = center + paperback::Vector3f{ 0.0f, .707f * radius, .707f * radius };
        top_left = center + paperback::Vector3f{ 0.0f, .707f * radius, -.707f * radius };
        right = center + paperback::Vector3f{ 0.0f, 0.0f, radius };
        left = center + paperback::Vector3f{ 0.0f, 0.0f, -radius };
        bottom_right = center + paperback::Vector3f{ 0.0f, -.707f * radius, .707f * radius };
        bottom_left = center + paperback::Vector3f{ 0.0f, -.707f * radius, -.707f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //circle for diagonal right facing
        top_right = center + paperback::Vector3f{ .5f * radius, .707f * radius, -.5f * radius };
        top_left = center + paperback::Vector3f{ -.5f * radius, .707f * radius, .5f * radius };
        right = center + paperback::Vector3f{ .707f * radius, 0.0f, -.707f * radius };
        left = center + paperback::Vector3f{ -.707f * radius, 0.0f, .707f * radius };
        bottom_right = center + paperback::Vector3f{ .5f * radius, -.707f * radius, -.5f * radius };
        bottom_left = center + paperback::Vector3f{ -.5f * radius, -.707f * radius, .5f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //circle for diagonal left facing
        top_right = center + paperback::Vector3f{ .5f * radius, .707f * radius, .5f * radius };
        top_left = center + paperback::Vector3f{ -.5f * radius, .707f * radius, -.5f * radius };
        right = center + paperback::Vector3f{ .707f * radius, 0.0f, .707f * radius };
        left = center + paperback::Vector3f{ -.707f * radius, 0.0f, -.707f * radius };
        bottom_right = center + paperback::Vector3f{ .5f * radius, -.707f * radius, .5f * radius };
        bottom_left = center + paperback::Vector3f{ -.5f * radius, -.707f * radius, -.5f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //horizontal top circle
        top = center + paperback::Vector3f{ 0.0f, .707f * radius, -.707f * radius };
        top_right = center + paperback::Vector3f{ .5f * radius, .707f * radius, -.5f * radius };
        right = center + paperback::Vector3f{ .707f * radius, .707f * radius, 0.0f };
        bottom_right = center + paperback::Vector3f{ .5f * radius, .707f * radius, .5f * radius };
        bottom = center + paperback::Vector3f{ 0.0f, .707f * radius, .707f * radius };
        bottom_left = center + paperback::Vector3f{ -.5f * radius, .707f * radius, .5f * radius };
        left = center + paperback::Vector3f{ -.707f * radius, .707f * radius, 0.0f };
        top_left = center + paperback::Vector3f{ -.5f * radius, .707f * radius, -.5f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //horizontal middle circle
        top = center + paperback::Vector3f{ 0.0f, 0.0f, -radius };
        top_right = center + paperback::Vector3f{ .707f * radius, 0.0f, -.707f * radius };
        right = center + paperback::Vector3f{ radius, 0.0f, 0.0f };
        bottom_right = center + paperback::Vector3f{ .707f * radius, 0.0f, .707f * radius };
        bottom = center + paperback::Vector3f{ 0.0f, 0.0f, radius };
        bottom_left = center + paperback::Vector3f{ -.707f * radius, 0.0f, .707f * radius };
        left = center + paperback::Vector3f{ -radius, 0.0f, 0.0f };
        top_left = center + paperback::Vector3f{ -.707f * radius, 0.0f, -.707f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);

        //horizontal bottom circle
        top = center + paperback::Vector3f{ 0.0f, -.707f * radius, -.707f * radius };
        top_right = center + paperback::Vector3f{ .5f * radius, -.707f * radius, -.5f * radius };
        right = center + paperback::Vector3f{ .707f * radius, -.707f * radius, 0.0f };
        bottom_right = center + paperback::Vector3f{ .5f * radius, -.707f * radius, .5f * radius };
        bottom = center + paperback::Vector3f{ 0.0f, -.707f * radius, .707f * radius };
        bottom_left = center + paperback::Vector3f{ -.5f * radius, -.707f * radius, .5f * radius };
        left = center + paperback::Vector3f{ -.707f * radius, -.707f * radius, 0.0f };
        top_left = center + paperback::Vector3f{ -.5f * radius, -.707f * radius, -.5f * radius };

        ConvertVerticesToCircleDraw(debugdraw, top, bottom, right, left, top_right, bottom_right, top_left, bottom_left);


        DrawDebugLines(debugdraw, Sphere.m_Collided);

        Sphere.m_Collided = false;
    }

    //given a set of vertices defining a square, prepare the pairing required
    void ConvertVerticesToSquareDraw(std::vector<paperback::Vector3f>& Vec,
        const paperback::Vector3f& Top_left,
        const paperback::Vector3f& Top_right,
        const paperback::Vector3f& Bottom_left,
        const paperback::Vector3f& Bottom_right)
    {

        Vec.push_back(Top_left);
        Vec.push_back(Top_right);

        Vec.push_back(Top_right);
        Vec.push_back(Bottom_right);

        Vec.push_back(Bottom_right);
        Vec.push_back(Bottom_left);

        Vec.push_back(Bottom_left);
        Vec.push_back(Top_left);
    }

    // draws a square given the provided data
    void DrawCubeCollision(boundingbox& Cube, transform& Transform)
    {

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f front_top_left, front_top_right, front_bottom_left, front_bottom_right,
            back_top_left, back_top_right, back_bottom_left, back_bottom_right, diff;

        front_top_right = front_top_left = front_bottom_left = front_bottom_right = Cube.Max + Transform.m_Position + Transform.m_Offset;
        back_bottom_left = back_top_left = back_top_right = back_bottom_right = Cube.Min + Transform.m_Position + Transform.m_Offset;
        diff = front_top_right - back_bottom_left;

        front_top_left -= paperback::Vector3f{ diff.x, 0.0f, 0.0f };
        front_bottom_left -= paperback::Vector3f{ diff.x, diff.y, 0.0f };
        front_bottom_right -= paperback::Vector3f{ 0.0f, diff.y, 0.0f };

        back_top_right += paperback::Vector3f{ diff.x, diff.y, 0.0f };
        back_top_left += paperback::Vector3f{ 0.0f, diff.y, 0.0f };
        back_bottom_right += paperback::Vector3f{ diff.x, 0.0f, 0.0f };

        //form 4 squares, front, back, top, bottom
        //front
        ConvertVerticesToSquareDraw(debugdraw, front_top_left, front_top_right, front_bottom_left, front_bottom_right);

        //back
        ConvertVerticesToSquareDraw(debugdraw, back_top_left, back_top_right, back_bottom_left, back_bottom_right);

        //top
        ConvertVerticesToSquareDraw(debugdraw, back_top_left, back_top_right, front_top_left, front_top_right);

        //bottom
        ConvertVerticesToSquareDraw(debugdraw, back_bottom_left, back_bottom_right, front_bottom_left, front_bottom_right);

        DrawDebugLines(debugdraw, Cube.m_Collided);

        Cube.m_Collided = false;
    }

    // draws a "cube" depending on given data
    // data has to be pairs of vectors
    void DrawDebugLines(std::vector<paperback::Vector3f> Vec, bool IsCollide = false)
    {

        //assume passed vector contains vector3f in pairs
        for (paperback::Vector3f& vec3 : Vec)
        {

            m_Points[IsCollide].push_back(ConvertMathVecToGLMVec(vec3));
        }
    }



    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {

        m_IsDebug = true;
    }


    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, transform& Transform, boundingbox* Cube, sphere* Ball) noexcept
    {

        if (m_IsDebug)
        {
        
            if (Cube)
                DrawCubeCollision(*Cube, Transform);

            if (Ball)
                DrawSphereCollision(*Ball, Transform);
        }
    }

    void DebugInputTest()
    {

        if (PPB.IsKeyPressDown(GLFW_KEY_J))
        {

            GetSystem<physics_system>().ApplyForceAll({ -1.0f, 0.0f, 0.0f });
        }
        if (PPB.IsKeyPressDown(GLFW_KEY_L))
        {

            GetSystem<physics_system>().ApplyForceAll({ 1.0f, 0.0f, 0.0f });
        }
        if (PPB.IsKeyPressDown(GLFW_KEY_I))
        {

            GetSystem<physics_system>().ApplyForceAll({ 0.0f, 0.0f, -1.0f });
        }
        if (PPB.IsKeyPressDown(GLFW_KEY_K))
        {

            GetSystem<physics_system>().ApplyForceAll({ 0.0f, 0.0f, 1.0f });
        }
        if (PPB.IsKeyPressDown(GLFW_KEY_O))
        {

            GetSystem<physics_system>().ApplyForceAll({ 0.0f, 1.0f, 0.0f });
        }
        if (PPB.IsKeyPressDown(GLFW_KEY_U))
        {

            GetSystem<physics_system>().ApplyForceAll({ 0.0f, -1.0f, 0.0f });
        }
    }

    const std::array<std::vector<glm::vec3>, 2>& GetPoints() const
    {
        return m_Points;
    }

    PPB_FORCEINLINE
    void OnFrameEnd(void) noexcept 
    {

        DebugInputTest();
        m_Points[0].clear();
        m_Points[1].clear();
    }

    //DrawPoint(const Vector3f& point)
    //{
    //    return DrawSphere(Sphere(point, 0.1f));
    //}
    //
    //DrawLine(const LineSegment& line)
    //{
    //    shape = GetNewShape();
    //    shape.mSegments.emplace_back(line);
    //    return shape;
    //}
    //
    //ComputeOrthogonalBasis(Vector3f& u, Vector3f& v, Vector3f& w)
    //{
    //    if (u.x == 0.f && u.y == 0.f)
    //        v = Vector3f(0.f, 1.0f, 0.f);
    //    else
    //    {
    //        v.x = -u.y;
    //        v.y = u.x;
    //        v.z = 0.f;
    //    }
    //    w = (u.Cross(v)).Normalized();
    //    v = v.Normalized();
    //}
    //
    //DrawCone(const Vector3f& point, const Vector3f& u, const Vector3f& v,
    //    const Vector3f& w, disc& d, int a)
    //{
    //    LineSegment line;
    //    const float angle = Math::cTwoPi / 40.f;
    //    float b = 0.f;
    //    for (int i = 0; i < 40; ++i)
    //    {
    //        line.mStart = point + cosf(b) * v + sinf(b) * w;
    //        line.mEnd = point + cosf(b + angle) * v + sinf(b + angle) * w;
    //        d.emplace_back(line);
    //        if ((i % a) == 0)
    //        {
    //            line.mEnd = point + u;
    //            d.emplace_back(line);
    //        }
    //        b += angle;
    //    }
    //}
    //DrawRay(const Ray& ray, float t)
    //{
    //    // Draw a ray to a given t-length. The ray must have an arrow head for visualization
    //    shape = GetNewShape();
    //    LineSegment line;
    //    line.mStart = ray.mStart;
    //    line.mEnd = ray.mStart + t * ray.mDirection;
    //    shape.mSegments.emplace_back(line);
    //
    //    // cone
    //    Vector3f u = ray.mDirection,
    //        v = Vector3f::cZero,
    //        w = Vector3f::cZero;
    //    ComputeOrthogonalBasis(u, v, w);
    //    DrawCone(line.mEnd - u, u, v, w, shape.mSegments, 2);
    //    return shape;
    //}
    //
    //DrawDisc(const Vector3f& point, const float& radius, const Vector3f& u,
    //    const Vector3f& v, const Vector3f& w, disc& d)
    //{
    //    LineSegment line;
    //    const float angle = Math::cTwoPi / 40.0f;
    //    float b = 0.f;
    //    for (int i = 0; i < 40; ++i)
    //    {
    //        line.mStart = point + radius * (cosf(b) * v + sinf(b) * w);
    //        line.mEnd = point + radius * (cosf(b + angle) * v + sinf(b + angle) * w);
    //        d.emplace_back(line);
    //        b += angle;
    //    }
    //}
    //
    //DrawSphere(const Sphere& sphere)
    //{
    //    // Draw a sphere with 4 rings: x-axis, y-axis, z-axis, and the horizon disc.
    //    shape = GetNewShape();
    //    Vector3f End;
    //
    //    if (mApplication)
    //        End = mApplication->mCamera.mTranslation;
    //    else
    //        End = Vector3f::cZero;
    //
    //    // distance of view to sphere
    //    Vector3f viewDir = sphere.mCenter - End;
    //    if (viewDir.LengthSq() <= 0.0f) // no vec
    //        return shape;
    //
    //    Vector3f norm_viewdir = viewDir.Normalized();
    //    float view_length = viewDir.Length();
    //    float rad_sq = sphere.mRadius * sphere.mRadius;
    //    float view_vec = sqrtf(view_length * view_length - rad_sq);
    //    float update_radius = sphere.mRadius * view_vec / view_length;
    //    float z = sqrtf(rad_sq - update_radius * update_radius);
    //    Vector3f update_center = sphere.mCenter - z * norm_viewdir;
    //
    //    // horizontal disc
    //    Vector3f u = viewDir, v, w;
    //    ComputeOrthogonalBasis(u, v, w);
    //    DrawDisc(update_center, update_radius, u, v, w, shape.mSegments);
    //
    //    // disc axis
    //    DrawDisc(sphere.mCenter, sphere.mRadius,
    //        Vector3f::cYAxis, Vector3f::cZAxis, Vector3f::cXAxis, shape.mSegments);
    //    DrawDisc(sphere.mCenter, sphere.mRadius,
    //        Vector3f::cZAxis, Vector3f::cXAxis, Vector3f::cYAxis, shape.mSegments);
    //    DrawDisc(sphere.mCenter, sphere.mRadius,
    //        Vector3f::cXAxis, Vector3f::cZAxis, Vector3f::cYAxis, shape.mSegments);
    //    return shape;
    //}
    //
    //DrawAabb(const Aabb& aabb)
    //{
    //    // Draw all edges of an aabb. Make sure to not mis-match edges!
    //    shape = GetNewShape();
    //    LineSegment line;
    //
    //    //vertical line
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
    //    line.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    //horizontal line
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    //inward line
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
    //    line.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
    //    shape.mSegments.emplace_back(line);
    //    return shape;
    //}
    //
    //DrawTriangle(const Triangle& triangle)
    //{
    //    // Draw the 3 edges of a triangles
    //    DebugShape& shape = GetNewShape();
    //    LineSegment line;
    //
    //    line.mStart = triangle.mPoints[0];
    //    line.mEnd = triangle.mPoints[1];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = triangle.mPoints[1];
    //    line.mEnd = triangle.mPoints[2];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = triangle.mPoints[2];
    //    line.mEnd = triangle.mPoints[0];
    //    shape.mSegments.emplace_back(line);
    //    return shape;
    //}
    //
    //DrawPlane(const Plane& plane, float sizeX, float sizeY)
    //{
    //    // Draw a quad with a normal at the plane's center.
    //    shape = GetNewShape();
    //    Vector3f plane_normal = Vector3f(plane.mData.x, plane.mData.y, plane.mData.z);
    //    Vector3f plane_point = Vector3f(plane_normal.x, plane_normal.y, plane_normal.z) * plane.mData.w;
    //    Vector3f vec_horizontal;
    //
    //    if (plane_normal != Vector3(1.0f, 0.0f, 0.0f))
    //        vec_horizontal = plane_normal.Cross(Vector3(1.0f, 0.0f, 0.0f));
    //    else vec_horizontal = plane_normal.Cross(Vector3(0.0f, 1.0f, 0.0f));
    //
    //    vec_horizontal.Normalize();
    //    Vector3 vec_vertical = vec_horizontal.Cross(plane_normal);
    //
    //    LineSegment line;
    //    line.mStart = plane_point + Vector3(vec_horizontal.x, vec_horizontal.y, vec_horizontal.z)
    //        + Vector3(vec_vertical.x, vec_vertical.y, vec_vertical.z);
    //    line.mEnd = plane_point + Vector3(vec_horizontal.x, vec_horizontal.y, vec_horizontal.z)
    //        - Vector3(vec_vertical.x, vec_vertical.y, vec_vertical.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = line.mEnd;
    //    line.mEnd = plane_point - Vector3(vec_horizontal.x, vec_horizontal.y, vec_horizontal.z)
    //        - Vector3(vec_vertical.x, vec_vertical.y, vec_vertical.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = line.mEnd;
    //    line.mEnd = plane_point - Vector3(vec_horizontal.x, vec_horizontal.y, vec_horizontal.z)
    //        + Vector3(vec_vertical.x, vec_vertical.y, vec_vertical.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = line.mEnd;
    //    line.mEnd = plane_point + Vector3(vec_horizontal.x, vec_horizontal.y, vec_horizontal.z)
    //        + Vector3(vec_vertical.x, vec_vertical.y, vec_vertical.z);
    //    shape.mSegments.emplace_back(line);
    //
    //    // normal
    //    line.mStart = plane_point;
    //    line.mEnd = plane_point + plane_normal;
    //    shape.mSegments.emplace_back(line);
    //
    //    return shape;
    //}
    //
   //DrawQuad(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
    //{
    //    // Draw the4 edges of a quad. Make sure to look at this and make sure the quad is not bow-tied.
    //    shape = GetNewShape();
    //    LineSegment line;
    //
    //    line.mStart = p0;
    //    line.mEnd = p1;
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = p1;
    //    line.mEnd = p2;
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = p2;
    //    line.mEnd = p3;
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = p3;
    //    line.mEnd = p0;
    //    shape.mSegments.emplace_back(line);
    //    return shape;
    //}
    //
    //DrawFrustum(const Frustum& frustum)
    //{
    //    // Draw the 6 faces of the frustum using the 8 frustum points.
    //    // See Frustum.Set for the point order. For example, Points[4] is left-bottom-front.
    //    shape = GetNewShape();
    //    LineSegment line;
    //
    //    // near plane
    //    line.mStart = frustum.mPoints[0];
    //    line.mEnd = frustum.mPoints[1];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[1];
    //    line.mEnd = frustum.mPoints[2];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[2];
    //    line.mEnd = frustum.mPoints[3];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[3];
    //    line.mEnd = frustum.mPoints[0];
    //    shape.mSegments.emplace_back(line);
    //
    //    // 4 lines (inward ones)
    //    line.mStart = frustum.mPoints[0];
    //    line.mEnd = frustum.mPoints[4];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[1];
    //    line.mEnd = frustum.mPoints[5];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[2];
    //    line.mEnd = frustum.mPoints[6];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[3];
    //    line.mEnd = frustum.mPoints[7];
    //    shape.mSegments.emplace_back(line);
    //
    //    // far plane
    //    line.mStart = frustum.mPoints[4];
    //    line.mEnd = frustum.mPoints[5];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[5];
    //    line.mEnd = frustum.mPoints[6];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[6];
    //    line.mEnd = frustum.mPoints[7];
    //    shape.mSegments.emplace_back(line);
    //
    //    line.mStart = frustum.mPoints[7];
    //    line.mEnd = frustum.mPoints[4];
    //    shape.mSegments.emplace_back(line);
    //    return shape;
    //}
};