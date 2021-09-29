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

    // draws a "cube" depending on given data
    // data has to be pairs of vectors
    void DrawDebugLines(std::vector<paperback::Vector3f> vec)
    {

        //assume passed vector contains vector3f in pairs

        //convert to glm vector
        std::vector<glm::vec3> input{};

        for (paperback::Vector3f& vec3 : vec)
        {

            input.push_back(ConvertMathVecToGLMVec(vec3));
        }

        // send to render
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
        //TestDrawDebug();
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