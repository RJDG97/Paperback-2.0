#pragma once
#include "Editor/Panels/ArchetypeInspector.h"

void ArchetypeInspector::Panel()
{
    int Index = 0;
    std::string ArchetypeName;

    ImGui::Begin(ArchetypeInspector::typedef_v.m_pName, &m_bEnabled);

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 170.0f);

    for (auto& Archetype : PPB.GetArchetypeList())
    {
        ++Index;

        char Buffer[256]{};
        ArchetypeName = Archetype->GetName();
        std::copy(ArchetypeName.begin(), ArchetypeName.end(), Buffer);

        if (Filter.PassFilter(ArchetypeName.c_str()))
        {
            ImGui::PushItemWidth(150.0f);

            if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                Archetype->SetName(std::string(Buffer));

            ImGui::PopItemWidth();

            if (ImGui::IsItemHovered()) //view components
            {
                m_Imgui.m_pArchetype = Archetype;
                m_Imgui.m_ComponentNames.clear();

                for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
                    m_Imgui.m_ComponentNames.push_back(Archetype->GetComponentInfos()[i]->m_pName);

                ImGui::BeginTooltip();

                if (!m_Imgui.m_ComponentNames.empty() && m_Imgui.m_pArchetype)
                {
                    ImGui::Text("Number of Entities: %d", m_Imgui.m_pArchetype->GetCurrentEntityCount());

                    ImGui::Separator();

                    ImGui::Text("Archetype Components: ");

                    for (auto& Names : m_Imgui.m_ComponentNames)
                        ImGui::Text(Names);
                }

                ImGui::EndTooltip();
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE " Spawn New Entity"))
                {
                    m_Imgui.m_pArchetype = Archetype;

                    if (m_Imgui.m_pArchetype)
                    {
                        m_Imgui.m_pArchetype->CreateEntity();
                        EDITOR_INFO_PRINT("Entity: A " + m_Imgui.m_pArchetype->GetName() + " is added to the scene");
                    }
                }

                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}