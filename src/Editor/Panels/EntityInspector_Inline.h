#include "Editor/Panels/EntityInspector.h"

void EntityInspector::InspectorWindow()
{
    int NumEntities = 0, Index = 0;
    bool b_NodeOpen = false;

    ImGui::Begin(EntityInspector::typedef_v.m_pName);

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 150.0f);

    if (!PPB.GetArchetypeList().empty())
    {
        for (auto& Archetype : PPB.GetArchetypeList())
        {
            Index++;

            if (Filter.PassFilter(Archetype->GetName().c_str()))
            {
                for (paperback::u32 i = 0; i < Archetype->GetEntityCount(); ++i)
                {
                    NumEntities++;

                    if (ImGui::Selectable((Archetype->GetName() + " [" + std::to_string(i) + "]").c_str() ))
                    {
                        m_Imgui.m_SelectedEntity.first = Archetype;
                        m_Imgui.m_SelectedEntity.second = i;
                        m_Imgui.m_Components = Archetype->GetEntityComponents(m_Imgui.m_SelectedEntity.second);

                    }

                    ImGui::SameLine();

                    if (ImGui::Button(ICON_FA_TRASH))
                    {
                        ImGui::OpenPopup(ICON_FA_TRASH " Delete?");

                        DeleteEntity(ICON_FA_TRASH " Delete?", i, (Archetype->GetName() + " [" + std::to_string(i) + "]"));
                    }
                }
            }
        }
    }

    ImGui::Separator();
    ImGui::Text("%d Entities", NumEntities);

    ImGui::End();
}

void EntityInspector::DeleteEntity(std::string WindowName, paperback::u32 EntityIndex, std::string EntityLabel)
{
    if (m_Imgui.m_SelectedEntity.first)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Deleting: ");

            ImGui::Text(EntityLabel.c_str());

            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "This cannot be undone");

            ImGui::Separator();
            
            if (ImGui::Button("Yes"))
            {
                PPB.DeleteEntity( m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, EntityIndex }) );

                m_Imgui.m_SelectedEntity = { nullptr, paperback::u32_max };

                if ( !m_Imgui.m_Components.empty() )
                    m_Imgui.m_Components.clear();

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine( 0, 4 ); 

            if ( ImGui::Button("Cancel") )
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

    }
}
