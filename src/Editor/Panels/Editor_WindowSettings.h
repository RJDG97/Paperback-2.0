#pragma once
#include "Editor/EditorInclude.h"
#include "Systems/WindowSystem.h"
#include "Json/paperback_json.h"

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
        //rttr::instance obj = GetSystem< window_system >().E;

        //for ( auto& Property : obj.get_type().get_properties() )
        //{
        //    rttr::variant PropertyValue = Property.get_value( obj );

        //    auto PropertyType = Property.get_type();
        //    auto PropertyName = Property.get_name().to_string();

        //    if ( PropertyType == rttr::type::get< std::reference_wrapper< int >() )
        //    {
        //        ImGui::Text( PropertyName.c_str() ); ImGui::SameLine();
        //        ImGui::PushItemWidth(); 
        //        if ( ImGui::InputInt( ("##" + PropertyName).c_str(), &(PropertyValue.get_value< std::reference_wrapper< int > >().get()), ImGuiInputTextFlags_EnterReturnsTrue))
        //        {
        //            glfwSetWindowSize( m_pWindow, GetSystem< window_system >().E.m_Width, GetSystem< window_system >().E.m_Height );
        //        }
        //    }

        //    if ( PropertyType == rttr::type::get< std::reference_wrapper< std::string > >())
        //    {
        //        auto& String = PropertyValue.get_value< std::reference_wrapper< std::string > >().get();
        //        char Buffer[256] {};

        //        std::copy( String.begin(), String.End(), Buffer );

        //        if ( ImGui::InputText( ("##" + PropertyName).c_str(), Buffer, sizeof(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        //        {
        //               String = std::string( Buffer );
        //               glfwSetWindowTitle( m_pWindow, GetSystem< window_system >().E.m_WinName );
        //        }
        //    }
        //}

        //if (ImGui::Button( "Save Changes" ))
        //{
        //    JFile.StartWriter( "../../resources/assetloading/config.json" ).ReadObjects( GetSystem<window_system>().E ).EndWriter();
        //}

        ImGui::End();
    }

};