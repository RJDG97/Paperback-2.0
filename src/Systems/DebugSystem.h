#pragma once
#include "Components/Transform.h"
#include "../Functionality/Renderer/Renderer.h"
#include "Math/Vector3f.h"

struct debug_system : paperback::system::instance
{

    const size_t m_NumSystems = 15;

    //container to keep system run time
    std::vector<std::chrono::high_resolution_clock::time_point> m_RawTimePerSystem;
    std::vector<float> m_PercentageTimePerSystem, m_TimePerSystem;
    float m_TotalTime;


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "debug_system"
    };

    // helper function to track timer
    // stores the then current time at the start of the system's run for the frame
    // to be called at OnFrameStart
    void BeginTime(size_t system_index)
    {
        std::chrono::high_resolution_clock::time_point temp = PPB.m_Clock.Now();
        m_RawTimePerSystem[system_index] = temp;
    }

    // helper function to track timer
    // stops the "timer" of the system and stores the time spent on the system
    void EndTime(size_t system_index)
    {

        //m_RawTimePerSystem[system_index] 
        m_TimePerSystem[system_index] = static_cast<std::chrono::duration<float>>(PPB.m_Clock.Now() - m_RawTimePerSystem[system_index]).count();
    }


    //debug print function
    // prints current total time against individual times by percentage
    void DebugPrint()
    {

        std::cout << "Total time: " << m_TotalTime << std::endl;
        std::cout << "\tSystem 1: " << m_PercentageTimePerSystem[0] << std::endl;
        std::cout << "\tSystem 2: " << m_PercentageTimePerSystem[1] << std::endl;
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
    void DrawSphereCollision(transform& xform, bool iscollide = false)
    {

        Sphere& sphere = xform.fakeSphere;

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f center = sphere.getCenter();
        paperback::Vector3f top = center;
        paperback::Vector3f bottom = top;

        float radius = sphere.getRadius();

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
        
        DrawDebugLines(debugdraw);
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
    void DrawCubeCollision(transform& transform, bool iscollide = false)
    {

        std::vector<paperback::Vector3f> debugdraw;

        paperback::Vector3f front_top_left, front_top_right, front_bottom_left, front_bottom_right,
                            back_top_left, back_top_right, back_bottom_left, back_bottom_right, diff;

        front_top_right = front_top_left = front_bottom_left = front_bottom_right = transform.fakebox.MinMax[1];
        back_bottom_left = back_top_left = back_top_right = back_bottom_right = transform.fakebox.MinMax[0];
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

        DrawDebugLines(debugdraw, iscollide);
    }

    // draws a "cube" depending on given data
    // data has to be pairs of vectors
    void DrawDebugLines(std::vector<paperback::Vector3f> vec, bool iscollide = false)
    {

        //assume passed vector contains vector3f in pairs

        //convert to glm vector
        std::vector<glm::vec3> input{};

        for (paperback::Vector3f& vec3 : vec)
        {

            input.push_back(ConvertMathVecToGLMVec(vec3));
        }

        // send to render
        if (!iscollide)
            Renderer::GetInstanced().DebugRender(input);
    }


    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        // set up data container to store data about systems
        m_RawTimePerSystem.resize(m_NumSystems);
        m_PercentageTimePerSystem.resize(m_NumSystems);
        m_TimePerSystem.resize(m_NumSystems);
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
    void Update(void) 
    {
        TestDrawDebug();
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
    }

    // Terminate system
    PPB_FORCEINLINE
    void OnSystemTerminated(void) noexcept 
    {

    }
};