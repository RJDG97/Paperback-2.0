#pragma once
#include "Components/Transform.h"
#include "../Functionality/Renderer/Renderer.h"
#include "Math/Vector3f.h"

//#include "Editor/EditorLogger.h"

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

    bool statetest = false;

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

        if (PPB.IsKeyPressUp(GLFW_KEY_B))
        {

            if (!statetest)
            {
                statetest = true;
                PPB.OpenScene("Editor");
                //EDITOR_TRACE_PRINT("Trial Scene Loaded");
            }
            else
            {
                statetest = false;
                PPB.OpenScene("Default");
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
};