#pragma once
#include "Components/Transform.h"
#include "../Functionality/Renderer/Renderer.h"
#include "Math/Vector3f.h"

struct debug_system : paperback::system::instance
{

    const size_t m_NumSystems = 15;

    std::array<std::vector<glm::vec3>, 2> m_Points;

    //container to keep system run time
    std::vector<std::chrono::high_resolution_clock::time_point> m_RawTimePerSystem;
    std::vector<float> m_PercentageTimePerSystem, m_TimePerSystem;
    float m_TotalTime;

    bool m_IsDebug;


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "debug_system"
    };

    // helper function to track timer
    // stores the then current time at the start of the system's run for the frame
    // to be called at OnFrameStart
    void BeginTime(size_t system_index)
    {
        std::chrono::high_resolution_clock::time_point temp = PPB.Now();
        m_RawTimePerSystem[system_index] = temp;
    }

    // helper function to track timer
    // stops the "timer" of the system and stores the time spent on the system
    void EndTime(size_t system_index)
    {

        m_TimePerSystem[system_index] = static_cast<std::chrono::duration<float>>(PPB.Now() - m_RawTimePerSystem[system_index]).count();
    }


    //debug print function
    // prints current total time against individual times by percentage
    void DebugPrint()
    {

        /*std::cout << "Total time: " << m_TotalTime << std::endl;
        std::cout << "\tSystem 1: " << m_PercentageTimePerSystem[0] << std::endl;
        std::cout << "\tSystem 2: " << m_PercentageTimePerSystem[1] << std::endl;*/
    }

    // returns a reference to the vector of time taken per system in seconds
    std::vector<float>& GetSystemTimeTaken()
    {

        return m_TimePerSystem;
    }

    //returns a reference to the vector of time taken per system in percentages
    std::vector<float>& GetSystemTimeTakenPercentage()
    {

        return m_PercentageTimePerSystem;
    }

    //helper function to convert xcore::vector3 to Vector3f
    paperback::Vector3f ConvertXcoreVecTo3f(const xcore::vector3& vec)
    {

        return { vec.m_X, vec.m_Y, vec.m_Z };
    }

    //helper function to convert Vector3f to glm::vec3
    glm::vec3 ConvertMathVecToGLMVec(const paperback::Vector3f& vec3)
    {

        return { vec3.x, vec3.y, vec3.z };
    }

    // given a set of points definining a circle, add to a vector the necessary connections to draw
    void ConvertVerticesToCircleDraw(std::vector<paperback::Vector3f>& vec,
        const paperback::Vector3f& top,
        const paperback::Vector3f& bottom,
        const paperback::Vector3f& right,
        const paperback::Vector3f& left,
        const paperback::Vector3f& top_right,
        const paperback::Vector3f& bottom_right,
        const paperback::Vector3f& top_left,
        const paperback::Vector3f& bottom_left)
    {

        vec.push_back(top);
        vec.push_back(top_right);

        vec.push_back(top_right);
        vec.push_back(right);

        vec.push_back(right);
        vec.push_back(bottom_right);

        vec.push_back(bottom_right);
        vec.push_back(bottom);

        vec.push_back(bottom);
        vec.push_back(bottom_left);

        vec.push_back(bottom_left);
        vec.push_back(left);

        vec.push_back(left);
        vec.push_back(top_left);

        vec.push_back(top_left);
        vec.push_back(top);
    }



    // draws a sphere given the provided data
    // low poly circle
    void DrawSphereCollision(Sphere& sphere, transform& transform)
    {

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f center = center.ConvertXcoreVecTo3f(transform.m_Position + transform.m_Offset);
        paperback::Vector3f top = center;
        paperback::Vector3f bottom = top;

        float radius = sphere.m_fRadius;

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


        DrawDebugLines(debugdraw, sphere.m_Collided);

        sphere.m_Collided = false;
    }

    //given a set of vertices defining a square, prepare the pairing required
    void ConvertVerticesToSquareDraw(std::vector<paperback::Vector3f>& vec,
        const paperback::Vector3f& top_left,
        const paperback::Vector3f& top_right,
        const paperback::Vector3f& bottom_left,
        const paperback::Vector3f& bottom_right)
    {

        vec.push_back(top_left);
        vec.push_back(top_right);

        vec.push_back(top_right);
        vec.push_back(bottom_right);

        vec.push_back(bottom_right);
        vec.push_back(bottom_left);

        vec.push_back(bottom_left);
        vec.push_back(top_left);
    }

    // draws a square given the provided data
    void DrawCubeCollision(BoundingBox& cube, transform& transform)
    {

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f front_top_left, front_top_right, front_bottom_left, front_bottom_right,
            back_top_left, back_top_right, back_bottom_left, back_bottom_right, diff;

        front_top_right = front_top_left = front_bottom_left = front_bottom_right = cube.Max + ConvertXcoreVecTo3f(transform.m_Position + transform.m_Offset);
        back_bottom_left = back_top_left = back_top_right = back_bottom_right = cube.Min + ConvertXcoreVecTo3f(transform.m_Position + transform.m_Offset);
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

        DrawDebugLines(debugdraw, cube.m_Collided);

        cube.m_Collided = false;
    }

    // draws a "cube" depending on given data
    // data has to be pairs of vectors
    void DrawDebugLines(std::vector<paperback::Vector3f> vec, bool iscollide = false)
    {

        //assume passed vector contains vector3f in pairs

        //convert to glm vector
        //std::vector<glm::vec3> input{};

        for (paperback::Vector3f& vec3 : vec)
        {

            m_Points[iscollide].push_back(ConvertMathVecToGLMVec(vec3));
        }

        // send to render
        //Renderer::GetInstanced().DebugRender(input, iscollide);
    }



    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        // set up data container to store data about systems
        m_RawTimePerSystem.resize(m_NumSystems);
        m_PercentageTimePerSystem.resize(m_NumSystems);
        m_TimePerSystem.resize(m_NumSystems);

        m_IsDebug = true;
    }


    //test debug draw function
    std::vector<paperback::Vector3f> testvec = { { -25, -25, -25 }, { 25, 25, 25 }, { 25, -25, -25 }, { -25, 25, 25 } };
    void TestDrawDebug()
    {

        //draw test vec
        DrawDebugLines(testvec);

        //test conversion of nonpaired vector

    }


    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, transform& Transform, BoundingBox* cube, Sphere* ball) noexcept
    {

        if (m_IsDebug)
        {
        
            if (cube)
                DrawCubeCollision(*cube, Transform);

            if (ball)
                DrawSphereCollision(*ball, Transform);
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
        //reset all raw times to 0 and update percentage values
        m_TotalTime = 0.0f;

        for (size_t i = 0; i < m_NumSystems; ++i)
        {

            m_PercentageTimePerSystem[i] = m_TimePerSystem[i];
            m_TotalTime += m_TimePerSystem[i];
        }

        for (float& percentage : m_PercentageTimePerSystem)
        {

            percentage /= m_TotalTime;
            percentage *= 100;
        }
        //DebugPrint();

        DebugInputTest();
        m_Points[0].clear();
        m_Points[1].clear();
    }

    // Terminate system
    PPB_FORCEINLINE
    void OnSystemTerminated(void) noexcept 
    {

    }
};