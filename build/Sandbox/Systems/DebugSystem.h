/**********************************************************************************
*\file         DebugSystem.h
*\brief        DebugSystem.h
*
*\author       Malcolm Lim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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

        float radius = Sphere.m_Radius;

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

    //given a set of vertices defining a triangle, prepare pairing required
    void ConvertVerticesToTriangleDraw(std::vector<paperback::Vector3f>& Vec,
        const paperback::Vector3f& Top,
        const paperback::Vector3f& Bottom_right,
        const paperback::Vector3f& Bottom_left)
    {

        Vec.push_back(Top);
        Vec.push_back(Bottom_right);

        Vec.push_back(Bottom_right);
        Vec.push_back(Bottom_left);

        Vec.push_back(Bottom_left);
        Vec.push_back(Top);
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

    void DrawCube(boundingbox& Cube, paperback::Vector3f& Position)
    {
        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f front_top_left, front_top_right, front_bottom_left, front_bottom_right,
            back_top_left, back_top_right, back_bottom_left, back_bottom_right, diff;

        front_top_right = front_top_left = front_bottom_left = front_bottom_right = Cube.Max + Position;
        back_bottom_left = back_top_left = back_top_right = back_bottom_right = Cube.Min + Position;
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

    void DrawSpline(paperback::Spline spline)
    {
        if (spline.m_Points.size())
        {
            //Draw sphere at every control point (*no collision, just using an existing function)
            for (size_t i = 0; i < spline.m_Points.size(); ++i)
            {
                sphere temp_sphere{ 0.05f, true };
                transform temp_transform{ {}, spline.m_Points[i].m_Point };
                DrawSphereCollision(temp_sphere, temp_transform);
            }

            for (float i = 0.0f; i < static_cast<float>(spline.m_Points.size() - 3) - 0.05f; i += 0.05f)
            {
                std::vector<paperback::Vector3f> render_points;
                render_points.push_back(spline.GetSplinePoint(i).m_Point);
                render_points.push_back(spline.GetSplinePoint(i + 0.05f).m_Point);
                GetSystem<debug_system>().DrawDebugLines(render_points, false);
            }
        }
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

    bool DrawSlope(boundingbox& Cube, transform& Transform, slope& Slope)
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

        //4 cases
        //north, south, east, west
        //draw side triangles + slope square
        if (Slope.m_North && !Slope.m_South && !Slope.m_East && !Slope.m_West)
        {

            //north facing slope, slope from -z to +z
            //left triangle
            ConvertVerticesToTriangleDraw(debugdraw, front_top_left, front_bottom_left, back_bottom_left);

            //right triangle
            ConvertVerticesToTriangleDraw(debugdraw, front_top_right, front_bottom_right, back_bottom_right);

            //square
            //ConvertVerticesToSquareDraw(debugdraw, front_top_left, front_top_right, back_bottom_left, back_bottom_right);
            
            //top line
            debugdraw.push_back(front_top_left);
            debugdraw.push_back(front_top_right);

            //bottom line
            debugdraw.push_back(back_bottom_left);
            debugdraw.push_back(back_bottom_right);
        }
        else if (!Slope.m_North && Slope.m_South && !Slope.m_East && !Slope.m_West)
        {

            //south facing slope, slope from +z to -z
            //left triangle
            ConvertVerticesToTriangleDraw(debugdraw, back_top_left, front_bottom_left, back_bottom_left);

            //right triangle
            ConvertVerticesToTriangleDraw(debugdraw, back_top_right, front_bottom_right, back_bottom_right);

            //square
            //ConvertVerticesToSquareDraw(debugdraw, back_top_left, back_top_right, front_bottom_right, front_bottom_left);
            
            //top line
            debugdraw.push_back(back_top_left);
            debugdraw.push_back(back_top_right);

            //bottom line
            debugdraw.push_back(front_bottom_left);
            debugdraw.push_back(front_bottom_right);
        }
        else if (!Slope.m_North && !Slope.m_South && Slope.m_East && !Slope.m_West)
        {

            //east facing slope, slope from -x to +x
            //left triangle
            ConvertVerticesToTriangleDraw(debugdraw, back_top_left, back_bottom_left, back_bottom_right);

            //right triangle
            ConvertVerticesToTriangleDraw(debugdraw, front_top_left, front_bottom_left, front_bottom_right);

            //square
            //ConvertVerticesToSquareDraw(debugdraw, front_top_left, back_top_left , back_bottom_right, front_bottom_right);

            //top line
            debugdraw.push_back(front_top_left);
            debugdraw.push_back(back_top_left);

            //bottom line
            debugdraw.push_back(front_bottom_right);
            debugdraw.push_back(back_bottom_right);
        }
        else if (!Slope.m_North && !Slope.m_South && !Slope.m_East && Slope.m_West)
        {

            //west facing slope, slope from +x to -x
            //left triangle
            ConvertVerticesToTriangleDraw(debugdraw, back_top_right, back_bottom_left, back_bottom_right);

            //right triangle
            ConvertVerticesToTriangleDraw(debugdraw, front_top_right, front_bottom_left, front_bottom_right);

            //square
            //ConvertVerticesToSquareDraw(debugdraw, front_top_right, back_top_right, back_bottom_left, front_bottom_left);

            //top line
            debugdraw.push_back(back_top_right);
            debugdraw.push_back(front_top_right);

            //bottom line
            debugdraw.push_back(back_bottom_left);
            debugdraw.push_back(front_bottom_left);
        }
        else
        {

            //all true or all false
            //return false if more than one or none is set to true
            return false;
        }

        DrawDebugLines(debugdraw, Cube.m_Collided);

        Cube.m_Collided = false;
        return true;
    }

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {

        m_IsDebug = false;
    }


    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, transform& Transform,
        boundingbox* Cube, sphere* Ball,
        LineSegment* line, Ray* ray,
        Triangle* triangle, Plane* plane, Frustum* frustum, slope* Slope
        ) noexcept
    {

        if (m_IsDebug)
        {
            // inefficient, draws 4 extrra lines
            //if (Cube)
            //    DrawCubeCollision(*Cube, Transform);

            if (Ball)
                DrawSphereCollision(*Ball, Transform);

            if (line)
                DrawLine(*line, Transform);
            if (Cube)
            {
                if (Slope && !DrawSlope(*Cube, Transform, *Slope))
                    DrawBoundingBox(*Cube, Transform);
                else if (!Slope)
                    DrawBoundingBox(*Cube, Transform);
            }
            if (triangle)
                DrawTriangle(*triangle, Transform);
            //// not in use
            //if ()
            //    DrawQuad();
            if (frustum)
                DrawFrustum(*frustum, Transform);
            if (ray)
                DrawRay(*ray, 5.f, Transform);
            if (plane)
                DrawPlane(*plane, 5.f, 5.f, Transform);

        }
    }

    void DebugInputTest()
    {

        if (PPB.VerifyState("Editor"))
            return;

        //if (PPB.IsKeyPressUp(GLFW_KEY_8))
        //{

        //    PPB.OpenScene("Editor");
        //    DEBUG_LOG("Debug access to Editor State");
        //}

        //if (PPB.IsKeyPressUp(GLFW_KEY_9))
        //{

        //    PPB.OpenScene("Workshop");
        //    DEBUG_LOG("Debug access to Workshop State");
        //}

        //if (PPB.IsKeyPressUp(GLFW_KEY_0))
        //{

        //    PPB.OpenScene("Combat");
        //    DEBUG_LOG("Debug access to Combat State");
        //}

        if (PPB.IsKeyPressDown(GLFW_KEY_LEFT_SHIFT) || PPB.IsKeyPressDown(GLFW_KEY_RIGHT_SHIFT))
        {

            if (PPB.IsKeyPressDown(GLFW_KEY_R))
            {

                PPB.QueueScene("MainMenu");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_T))
            {

                PPB.QueueScene("Cutscene_First");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_Y))
            {

                PPB.QueueScene("LoadingOne");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_U))
            {

                PPB.QueueScene("LoadingTwo");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_I))
            {

                PPB.QueueScene("Cutscene_Second");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_O))
            {

                PPB.QueueScene("LoadingThree");
            }
            else if (PPB.IsKeyPressDown(GLFW_KEY_P))
            {

                PPB.QueueScene("Cutscene_Third");
            }
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

    PPB_INLINE
        void OnStateChange(void) noexcept
    {
         // m_IsDebug = (PPB.VerifyState("Editor")) ? true : false;
    }


    paperback::Vector3f axis_xz_rot{ 0.f, 0.f, -1.f };
    paperback::Vector3f axis_yz_rot{ 1.f, 0.f, 0.f };
    paperback::Vector3f axis_xy_rot{ 0.f, 1.f, 0.f }; 
    const float PI = 3.14159f;

    PPB_INLINE
    paperback::Vector3f DirtyRotationAnglesFromDirectionalVec(const paperback::Vector3f& Directional)
    {
    
        paperback::Vector3f xz{}, yz{}, xy{};
        
        //xz
        xz.x = Directional.x;
        xz.z = Directional.z;
        
        //yz
        yz.y = Directional.y;
        yz.z = Directional.z;

        //xy
        xy.x = Directional.x;
        xy.y = Directional.y;

        //using xz component, get Y rotational value
        //project xz component onto X-Z plane, with 0 starting from (0, 0, -1) serving as "north"
        float dot = xz.Dot(axis_xz_rot);
        float det = xz.x * axis_xz_rot.z - xz.z * axis_xz_rot.x;
        float angle_y = atan2(det, dot);

        //using yz component, get X rotational value
        //project yz component onto y-z plane, with 0 starting from (1, 0, 0) serving as "north"
        dot = yz.Dot(axis_yz_rot);
        det = yz.y * axis_yz_rot.z - yz.z * axis_yz_rot.y;
        float angle_x = atan2(det, dot);

        //using xy component, get Z rotational value
        //project xy component onto x-y plane, with 0 starting from (0, 1, 0) serving as "north"
        dot = xy.Dot(axis_xy_rot);
        det = xy.x * axis_xy_rot.y - xy.y * axis_xy_rot.x;
        float angle_z = atan2(det, dot);

        return { angle_x / PI * 180, angle_y / PI * 180, angle_z / PI * 180 };
    }

    //----------------------------------------------------------------
    // Passes in a vector of LineSegment to draw Shape
    void DebugDrawLineSegment(std::vector<LineSegment> lines, bool IsCollide = false)
    {
        for (LineSegment& line : lines)
        {
            m_Points[IsCollide].push_back(ConvertMathVecToGLMVec(line.m_Start));
            m_Points[IsCollide].push_back(ConvertMathVecToGLMVec(line.m_End));
        }
    }

    // Returns the orthonormal basis
    void ComputeOrthogonalBasis(paperback::Vector3f& u, paperback::Vector3f& v, paperback::Vector3f& w)
    {
        if (u.x == 0.f && u.y == 0.f)
            v = paperback::Vector3f(0.f, 1.0f, 0.f);
        else
        {
            v.x = -u.y;
            v.y = u.x;
            v.z = 0.f;
        }

        w = (u.Cross(v)).Normalized();
        v.Normalize();
        u.Normalize();
    }

    void DrawLine(LineSegment& line, transform& Transform)
    {
        std::vector<LineSegment> shape;
        shape.emplace_back(LineSegment(
            line.m_Start + Transform.m_Position + Transform.m_Offset,
            line.m_End + Transform.m_Position + Transform.m_Offset,
            line.m_Collided));
        DebugDrawLineSegment(shape, line.m_Collided);

        // seems unnecessary part
        line.m_Collided = false;
    }

    void DrawBoundingBox(boundingbox& bbox, transform& Transform)
    {
        std::vector<LineSegment> shape;
        boundingbox cube(
            bbox.Min + Transform.m_Position + Transform.m_Offset,
            bbox.Max + Transform.m_Position + Transform.m_Offset,
            bbox.m_Collided);

        //vertical line
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Min.z), paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Min.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Min.z), paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Min.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Max.z), paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Max.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Max.z), paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Max.z), cube.m_Collided));

        //horizontal line
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Min.z), paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Min.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Max.z), paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Max.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Min.z), paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Min.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Max.z), paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Max.z), cube.m_Collided));

        //inward line
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Min.z), paperback::Vector3f(cube.Min.x, cube.Min.y, cube.Max.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Min.z), paperback::Vector3f(cube.Max.x, cube.Min.y, cube.Max.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Min.z), paperback::Vector3f(cube.Min.x, cube.Max.y, cube.Max.z), cube.m_Collided));
        shape.emplace_back(LineSegment(paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Min.z), paperback::Vector3f(cube.Max.x, cube.Max.y, cube.Max.z), cube.m_Collided));

        DebugDrawLineSegment(shape, cube.m_Collided);

        // seems unnecessary part
        bbox.m_Collided = false;
    }

    void DrawTriangle(Triangle& triangle, transform& Transform)
    {
        // Draw 3 edges of a triangle
        std::vector<LineSegment> shape;

        Triangle tri{
            triangle.m_pointA + Transform.m_Position + Transform.m_Offset,
            triangle.m_pointB + Transform.m_Position + Transform.m_Offset,
            triangle.m_pointC + Transform.m_Position + Transform.m_Offset,
            triangle.m_Collided };

        // Draw the 3 edges
        shape.emplace_back(LineSegment(tri.m_pointA, tri.m_pointB, tri.m_Collided));
        shape.emplace_back(LineSegment(tri.m_pointB, tri.m_pointC, tri.m_Collided));
        shape.emplace_back(LineSegment(tri.m_pointC, tri.m_pointA, tri.m_Collided));

        DebugDrawLineSegment(shape, tri.m_Collided);

        // seems unnecessary part
        triangle.m_Collided = false;
    }

    void DrawQuad(
        paperback::Vector3f& p0,
        paperback::Vector3f& p1,
        paperback::Vector3f& p2,
        paperback::Vector3f& p3,
        bool& collide,
        transform& Transform)
    {
        std::vector<LineSegment> shape;

        // Draw 4 edges
        shape.emplace_back(LineSegment(p0 + Transform.m_Position + Transform.m_Offset, p1 + Transform.m_Position + Transform.m_Offset));
        shape.emplace_back(LineSegment(p1 + Transform.m_Position + Transform.m_Offset, p2 + Transform.m_Position + Transform.m_Offset));
        shape.emplace_back(LineSegment(p2 + Transform.m_Position + Transform.m_Offset, p3 + Transform.m_Position + Transform.m_Offset));
        shape.emplace_back(LineSegment(p3 + Transform.m_Position + Transform.m_Offset, p0 + Transform.m_Position + Transform.m_Offset));

        DebugDrawLineSegment(shape, collide);

        // seems unnecessary part
        collide = false;
    }

    void DrawFrustum(Frustum& frustum, transform& Transform)
    {
        // Draw 6 faces of frustum using the 8 frustum points
        std::vector<LineSegment> shape;

        Frustum frus{
        frustum.m_pointA + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointB + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointC + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointD + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointE + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointF + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointG + Transform.m_Position + Transform.m_Offset,
        frustum.m_pointH + Transform.m_Position + Transform.m_Offset,
        frustum.m_Collided
        };

        // Near plane
        shape.emplace_back(LineSegment(frus.m_pointA, frus.m_pointB, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointB, frus.m_pointC, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointC, frus.m_pointD, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointD, frus.m_pointA, frus.m_Collided));

        // Connecting lines
        shape.emplace_back(LineSegment(frus.m_pointA, frus.m_pointE, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointB, frus.m_pointF, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointC, frus.m_pointG, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointD, frus.m_pointH, frus.m_Collided));

        // Far plane
        shape.emplace_back(LineSegment(frus.m_pointE, frus.m_pointF, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointF, frus.m_pointG, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointG, frus.m_pointH, frus.m_Collided));
        shape.emplace_back(LineSegment(frus.m_pointH, frus.m_pointE, frus.m_Collided));

        DebugDrawLineSegment(shape, frus.m_Collided);

        // seems unnecessary part
        frustum.m_Collided = false;
    }

    // Draw ray (used for plane normal drawing)
    void DrawArrow(Ray& ray, float t, transform& Transform, std::vector<LineSegment>& shape)
    {
        // Line of the ray
        LineSegment line(
            ray.m_Start + Transform.m_Position + Transform.m_Offset,
            ray.m_Start + t * ray.m_Direction + Transform.m_Position + Transform.m_Offset,
            ray.m_Collided);
        shape.emplace_back(line);

        // Compute orthonormal basis
        paperback::Vector3f u = ray.m_Direction, v, w;
        ComputeOrthogonalBasis(u, v, w);

        // Arrow head drawing, get delta angle per segment
        int segments = 40;
        const float delta = 2.f * PIf / segments;
        float angle = 0.f;

        // Size of arrow head              <radius, height>
        std::pair<float, float> arrowSz = { 0.25f, 0.5f };

        // Start point of arrow head disc
        paperback::Vector3f p = line.m_End - u * arrowSz.second;

        // Draw a cone for arrow head
        for (int i = 0; i < segments; ++i)
        {
            // Add line segment of arrowhead disc border
            LineSegment currLine(
                p + cosf(angle) * v * arrowSz.first + sinf(angle) * w * arrowSz.first,
                p + cosf(angle + delta) * v * arrowSz.first + sinf(angle + delta) * w * arrowSz.first,
                ray.m_Collided);

            shape.emplace_back(currLine);

            // End of ray
            if (!(i % 2))
            {
                currLine.m_End = p + u * arrowSz.second;
                shape.emplace_back(currLine);
            }
            angle += delta;
        }
    }

    // how to fix editor picking issue:
    // screen space vs world space
    // the cursor coordinates and the screen coordinates DO NOT MATCH
    // Solution:
    // 1: print cursor pos at max and min
    // 2: verify max and min of the screen space coordinates
    // 3: make sure cursor 0,0 matches screen 0,0
    // cursor 0,0 is TOP LEFT by default
    void DrawRay(Ray& ray, float t, transform& Transform)
    {
        // Draw a ray given time length
        std::vector<LineSegment> shape;
        DrawArrow(ray, t, Transform, shape);

        DebugDrawLineSegment(shape, ray.m_Collided);

        // seems unnecessary part
        ray.m_Collided = false;
    }

    void DrawPlane(Plane& plane, float sizeX, float sizeY, transform& Transform)
    {
        // Draw quad with a normal at plane's center
        std::vector<LineSegment> shape;

        // Get n and p0
        paperback::Vector3f n =
            paperback::Vector3f(plane.m_data.x, plane.m_data.y, plane.m_data.z);
        paperback::Vector3f p0 = n * plane.m_data.w;

        // Get v and w
        paperback::Vector3f v, w;
        ComputeOrthogonalBasis(n, v, w);

        // Construct the plane
        v *= sizeX, w *= sizeY;
        shape.emplace_back(LineSegment(p0 + v + w + Transform.m_Position + Transform.m_Offset, p0 + v - w + Transform.m_Position + Transform.m_Offset, plane.m_Collided));
        shape.emplace_back(LineSegment(p0 + v - w + Transform.m_Position + Transform.m_Offset, p0 - v - w + Transform.m_Position + Transform.m_Offset, plane.m_Collided));
        shape.emplace_back(LineSegment(p0 - v - w + Transform.m_Position + Transform.m_Offset, p0 - v + w + Transform.m_Position + Transform.m_Offset, plane.m_Collided));
        shape.emplace_back(LineSegment(p0 - v + w + Transform.m_Position + Transform.m_Offset, p0 + v + w + Transform.m_Position + Transform.m_Offset, plane.m_Collided));

        // Construct the normal ray
        Ray ray(p0, n, plane.m_Collided);
        DrawArrow(ray, 1.f, Transform, shape);

        DebugDrawLineSegment(shape, plane.m_Collided);

        // seems unnecessary part
        plane.m_Collided = false;
    }

    //----------------------------------------------------------------
    // to be added later after camera manipulation
};