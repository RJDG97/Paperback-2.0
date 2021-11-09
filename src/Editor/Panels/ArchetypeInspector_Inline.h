#pragma once
#include "Editor/Panels/ArchetypeInspector.h"

void ArchetypeInspector::Panel()
{
    ImGui::Begin(ArchetypeInspector::typedef_v.m_pName, &m_bEnabled);

    PrefabPanel();
    ArchetypePanel();

    ImGui::End();
}

void ArchetypeInspector::PrefabPanel()
{
    ImGui::BeginChild("Prefabs", { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y / 2 }, true, ImGuiWindowFlags_MenuBar);

    int Index = 0;
    std::string ArchetypeName;


    ImGui::BeginMenuBar();

    if (ImGui::MenuItem("Create Default Prefab"))
    {
        PPB.CreatePrefab();
        EDITOR_INFO_PRINT("Created: A Default Prefab");

    }

    ImGui::EndMenuBar();

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 170.0f);

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Default Prefab"))
        {
            PPB.CreatePrefab();
            EDITOR_INFO_PRINT("Created: A Default Prefab");
        }

        ImGui::EndPopup();
    }

    for (auto& Archetype : PPB.GetArchetypeList())
    {
        ++Index;
        for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
        {
            char Buffer[256]{};
            ArchetypeName = Archetype->GetName() + " [" + std::to_string(i) + "]";
            std::copy(ArchetypeName.begin(), ArchetypeName.end(), Buffer);

            if (Filter.PassFilter(ArchetypeName.c_str()))
            {
                //m_Imgui.ImguiHelp("Click to edit, Right Click on the prefab to spawn, on empty space for a default prefab, double click to edit");
                if (Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
                {
                    if (ImGui::InputText(("##ArchetypeName" +Archetype->GetName() + " [" + std::to_string(i) + std::to_string(Index) + "]").c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                        Archetype->SetName(std::string(Buffer));

                    if (ImGui::IsItemHovered()) //view components
                    {
                        m_Imgui.m_pArchetype = Archetype;
                        m_Imgui.m_ComponentNames.clear();

                        for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
                            m_Imgui.m_ComponentNames.push_back(Archetype->GetComponentInfos()[i]->m_pName);

                        ImGui::BeginTooltip();

                        if (!m_Imgui.m_ComponentNames.empty() && m_Imgui.m_pArchetype)
                        {
                            auto Prefab = m_Imgui.m_pArchetype->FindComponent<prefab>(paperback::vm::PoolDetails{ 0, i });

                            if (Prefab)
                                if(Prefab->m_ReferencePrefabGIDs.size())
                                    ImGui::Text("Number of Instances: %d", Prefab->m_ReferencePrefabGIDs.size());

                            ImGui::Separator();

                            ImGui::Text("Archetype Components: ");

                            for (auto& Names : m_Imgui.m_ComponentNames)
                                ImGui::Text(Names);
                        }

                        ImGui::EndTooltip();

                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            m_Imgui.m_SelectedEntity.first = Archetype;
                            m_Imgui.m_SelectedEntity.second = i;

                            m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
                            EDITOR_WARN_PRINT("Editing Prefab: " + Archetype->GetName());
                        }

                    }

                    if (ImGui::BeginPopupContextItem())
                    {
                        if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE " Clone Prefab"))
                        {
                            auto& Entity = m_Imgui.m_pArchetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i }); //first instance

                            auto& EntityInfo = PPB.GetEntityInfo(Entity.m_GlobalIndex);

                            m_Imgui.m_pArchetype->ClonePrefab(EntityInfo.m_PoolDetails.m_PoolIndex);

                            EDITOR_INFO_PRINT("Prefab Instance:" + m_Imgui.m_pArchetype->GetName() + " is added to the scene");
                        }

                        m_Imgui.ImGuiHelp("Spawns an instance of this prefab");

                        ImGui::EndPopup();
                    }
                }
            }

            //else
            //{
            //    if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            //        Archetype->SetName(std::string(Buffer));


            //    if (ImGui::IsItemHovered()) //view components
            //    {
            //        m_Imgui.m_pArchetype = Archetype;
            //        m_Imgui.m_ComponentNames.clear();

            //        for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
            //            m_Imgui.m_ComponentNames.push_back(Archetype->GetComponentInfos()[i]->m_pName);

            //        ImGui::BeginTooltip();

            //        if (!m_Imgui.m_ComponentNames.empty() && m_Imgui.m_pArchetype)
            //        {
            //            ImGui::Text("Number of Entities: %d", m_Imgui.m_pArchetype->GetCurrentEntityCount());

            //            ImGui::Separator();

            //            ImGui::Text("Archetype Components: ");

            //            for (auto& Names : m_Imgui.m_ComponentNames)
            //                ImGui::Text(Names);
            //        }

            //        ImGui::EndTooltip();
            //    }

            //    if (ImGui::BeginPopupContextItem())
            //    {

            //        if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE " Spawn New Entity"))
            //        {
            //            m_Imgui.m_pArchetype = Archetype;

            //            if (m_Imgui.m_pArchetype)
            //            {
            //                m_Imgui.m_pArchetype->CloneEntity();
            //                EDITOR_INFO_PRINT("Entity: A " + m_Imgui.m_pArchetype->GetName() + " is added to the scene");
            //            }
            //        }
            //        ImGui::EndPopup();
            //    }
            //}
        }
    }
    ImGui::EndChild();
}

void ArchetypeInspector::ArchetypePanel()
{
    ImGui::BeginChild("Files", { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y}, true);

    int Index = 0;
    std::string ArchetypeName;

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
            if (!(Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID)))
            {
                if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                    Archetype->SetName(std::string(Buffer));

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
            }
        }
    }

    ImGui::EndChild();
}