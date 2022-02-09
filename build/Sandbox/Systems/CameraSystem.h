#pragma once
#include "../src/paperback_camera.h"

struct camera_system : paperback::system::instance
{
    cam* m_pCamera;

    tools::query Query;

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "camera_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<paperback::input::manager::KeyPressed>(this);      // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<paperback::input::manager::KeyClicked>(this);      // Released                 ( Passes True )
        RegisterGlobalEventClass<paperback::input::manager::MousePressed>(this);    // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<paperback::input::manager::MouseClicked>(this);

        m_pCamera = &cam::GetInstanced();

        Query.m_Must.AddFromComponents<transform, camera>();
        Query.m_NoneOf.AddFromComponents<prefab>();
    }

    PPB_FORCEINLINE
    void Update( void ) noexcept
    {
        // Populate map
        ForEach(Search(Query), [&](transform& Transform, camera& Camera) noexcept
        {
            // temp offset on y axis
            glm::vec3 position = glm::vec3{ Transform.m_Position.x + Camera.m_Offset.x, Transform.m_Position.y + Camera.m_Offset.y, Transform.m_Position.z + Camera.m_Offset.z };

            Camera.UpdateView(position);
        });
    }

    // On Event Key / Mouse Pressed
    void OnEvent(const size_t& Key, const bool& Clicked) noexcept
    {
        if (Key == GLFW_MOUSE_BUTTON_RIGHT && !Clicked)
        {
            auto direction = GetMouseDirection();

            m_pCamera->RotateWithMouse(direction);
        }
    }
};