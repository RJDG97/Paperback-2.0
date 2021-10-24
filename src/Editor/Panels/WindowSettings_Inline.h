#pragma once
#include "Editor//Panels/WindowSettings.h"
#include "Functionality/Renderer/Renderer.h"

void WindowSettings::Panel()
{
    ImGui::Begin(WindowSettings::typedef_v.m_pName);

    rttr::instance obj = PPB.GetSystem< window_system >().E;

    for (auto& Property : obj.get_type().get_properties())
    {
        rttr::variant PropertyValue = Property.get_value(obj);

        auto PropertyType = Property.get_type();
        auto PropertyName = Property.get_name().to_string();

        if (PropertyType.get_wrapped_type().is_arithmetic())
            m_Imgui.DisplayBaseTypes(PropertyName, PropertyType, PropertyValue);
        else if (PropertyType.get_wrapped_type() == rttr::type::get< std::string >())
            m_Imgui.DisplayStringType(PropertyName, PropertyType, PropertyValue);

        glfwSetWindowSize(m_pWindow, PPB.GetSystem< window_system >().E.m_Width, PPB.GetSystem< window_system >().E.m_Height);
        Renderer::GetInstanced().UpdateFramebufferSize(PPB.GetSystem< window_system >().E.m_Width, PPB.GetSystem< window_system >().E.m_Height);

        glfwSetWindowTitle(m_pWindow, PPB.GetSystem< window_system >().E.m_WinName.c_str());
    }

    if (ImGui::Button("Save Changes"))
    {
        JFile.StartWriter("../../resources/assetloading/config.json").ReadObjects(PPB.GetSystem<window_system>().E).EndWriter();
        EDITOR_INFO_PRINT("Changes saved in Config.json");
    }

    ImGui::End();


}

