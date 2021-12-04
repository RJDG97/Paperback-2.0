#pragma once
#include "../src/paperback_camera.h"

struct camera_system : paperback::system::instance
{
    Camera3D* m_pCamera;

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "camera_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>(this);      // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<Input::KeyClicked>(this);      // Released                 ( Passes True )
        RegisterGlobalEventClass<Input::MousePressed>(this);    // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<Input::MouseClicked>(this);

        m_pCamera = &Camera3D::GetInstanced();
    }

    PPB_FORCEINLINE
    void Update( void ) noexcept
    {

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