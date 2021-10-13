#pragma once
#include "Editor/EditorInclude.h"
#include "Systems/WindowSystem.h"
#include "Json/paperback_json.h"
#include <rttr/type>

struct WindowSettings : paperback::editor::instance
{
    paperback::JsonFile JFile;
    GLFWwindow* m_pWindow;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Window Settings"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = false;
        m_pWindow = PPB.GetSystem<window_system>().m_pWindow;
    }

    void Update(void) noexcept
    {
        ImGui::Begin(WindowSettings::typedef_v.m_pName);

        rttr::instance obj = PPB.GetSystem< window_system >().E;

        for ( auto& Property : obj.get_type().get_properties() )
        {
            rttr::variant PropertyValue = Property.get_value( obj );

            auto PropertyType = Property.get_type();
            auto PropertyName = Property.get_name().to_string();

            if (PropertyType == rttr::type::get<std::reference_wrapper<int>>())
            {
                ImGui::Text( PropertyName.c_str() ); ImGui::SameLine();
                ImGui::PushItemWidth(100.0f); 
                if ( ImGui::InputInt( ("##" + PropertyName).c_str(), &(PropertyValue.get_value<std::reference_wrapper<int>>().get()), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    glfwSetWindowSize( m_pWindow, PPB.GetSystem< window_system >().E.m_Width, PPB.GetSystem< window_system >().E.m_Height );
                }

                ImGui::SameLine();

                ImGui::PopItemWidth();
            }

            if (PropertyType == rttr::type::get<std::reference_wrapper<std::string>>())
            {
                auto& str = PropertyValue.get_value<std::reference_wrapper<std::string>>().get();
                char buffer[256]{};

                std::copy(str.begin(), str.end(), buffer);

                if ( ImGui::InputText( ("##" + PropertyName).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    str = std::string(buffer);
                       glfwSetWindowTitle( m_pWindow, PPB.GetSystem< window_system >().E.m_WinName.c_str() );
                }
            }
        }

        if (ImGui::Button( "Save Changes" ))
        {
            JFile.StartWriter( "../../resources/assetloading/config.json" ).ReadObjects( PPB.GetSystem<window_system>().E ).EndWriter();
        }

        ImGui::End();
    }

};