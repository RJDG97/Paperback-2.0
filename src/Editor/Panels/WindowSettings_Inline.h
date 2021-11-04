#pragma once
#include "WindowSettings.h"

void WindowSettings::Panel()
{
	ImGui::Begin( WindowSettings::typedef_v.m_pName, &m_bEnabled);

	auto& WindowDetails = PPB.GetSystem< window_system >().E;

    auto& WinWidth = WindowDetails.m_Width;
    auto& WinHeight = WindowDetails.m_Height;
    auto& WinName = WindowDetails.m_WinName;

    char buffer[256]{};

    std::copy(WinName.begin(), WinName.end(), buffer);

    if (ImGui::InputText("##WindowName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        WindowDetails.m_WinName = std::string(buffer);
        EDITOR_DEBUG_PRINT(WindowDetails.m_WinName);
        glfwSetWindowTitle(m_pWindow, WindowDetails.m_WinName.c_str());
    }


    ImGui::PushItemWidth(100.0f);
     
    ImGui::Text("Window Width"); ImGui::SameLine();

    if (ImGui::InputInt("##WinWidth", &WinWidth, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        WindowDetails.m_Width = WinWidth;

        EDITOR_DEBUG_PRINT(WindowDetails.m_Width);

        glfwSetWindowSize(m_pWindow, WindowDetails.m_Width, WindowDetails.m_Height);
        Renderer::GetInstanced().UpdateFramebufferSize(WindowDetails.m_Width, WindowDetails.m_Height);
    }

    ImGui::SameLine();
    ImGui::Text("Window Height"); ImGui::SameLine();

    if ( ImGui::InputInt("##WinHeight", &WinHeight, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue) )
    {
        WindowDetails.m_Height = WinHeight;

        EDITOR_DEBUG_PRINT(WindowDetails.m_Height);

        glfwSetWindowSize(m_pWindow, WindowDetails.m_Width, WindowDetails.m_Height);
        Renderer::GetInstanced().UpdateFramebufferSize(WindowDetails.m_Width, WindowDetails.m_Height);

    }

    ImGui::PopItemWidth();

    //for (auto& Property : obj.get_type().get_properties())
    //{
    //    rttr::variant PropertyValue = Property.get_value(obj);

    //    auto PropertyType = Property.get_type();
    //    auto PropertyName = Property.get_name().to_string();

    //    if ( PropertyType == rttr::type::get<std::reference_wrapper<int>>() )
    //    {
    //        ImGui::Text(PropertyName.c_str()); ImGui::SameLine();
    //        ImGui::PushItemWidth(100.0f);

    //        auto& = 

    //        if (ImGui::InputInt(("##" + PropertyName).c_str(), &(PropertyValue.get_value<std::reference_wrapper<int>>().get()), ImGuiInputTextFlags_EnterReturnsTrue))
    //        {
    //            EDITOR_DEBUG_PRINT(PPB.GetSystem< window_system >().E.m_Height);
    //            EDITOR_DEBUG_PRINT(PPB.GetSystem< window_system >().E.m_Width);
    //            glfwSetWindowSize(m_pWindow, PPB.GetSystem< window_system >().E.m_Width, PPB.GetSystem< window_system >().E.m_Height);
    //            Renderer::GetInstanced().UpdateFramebufferSize(PPB.GetSystem< window_system >().E.m_Width, PPB.GetSystem< window_system >().E.m_Height);
    //        }

    //        ImGui::SameLine();

    //        ImGui::PopItemWidth();
    //    }

    //    if ( PropertyType == rttr::type::get<std::reference_wrapper<std::string>>() )
    //    {
    //        auto& str = PropertyValue.get_value<std::reference_wrapper<std::string>>().get();
    //        char buffer[256]{};

    //        std::copy(str.begin(), str.end(), buffer);

    //        if (ImGui::InputText(("##" + PropertyName).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    //        {
    //            str = std::string(buffer);
    //            glfwSetWindowTitle(m_pWindow, PPB.GetSystem< window_system >().E.m_WinName.c_str());
    //        }
    //    }
    //}
    ImGui::NewLine();

    if (ImGui::Button("Save Changes"))
    {
        JFile.StartWriter("../../resources/assetloading/config.json").ReadObjects(PPB.GetSystem<window_system>().E).EndWriter();
        EDITOR_INFO_PRINT("Changes saved in Config.json");
    }

	ImGui::End();

}