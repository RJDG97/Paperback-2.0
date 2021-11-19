#pragma once
#include "Editor/Panels/ArchetypeInspector.h"
#include "Systems/ImguiSystem.h"

void ArchetypeInspector::Panel()
{
    ImGui::Begin(ArchetypeInspector::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_MenuBar);

    MenuBar();

    PrefabPanel();
    //ArchetypePanel();

    ImGui::End();
}

void ArchetypeInspector::MenuBar()
{
    ImGui::BeginMenuBar();

    if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE))
    {
        PPB.CreatePrefab();
        EDITOR_INFO_PRINT("Added a Default Prefab");
    }

    m_Imgui.ImGuiHelp("Spawns a Default Prefab");

    if (ImGui::MenuItem(ICON_FA_SAVE))
        m_Imgui.m_Type = FileActivity::SAVEPREFAB;

    m_Imgui.ImGuiHelp("Saves All Prefabs");

    if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN))
        m_Imgui.m_Type = FileActivity::LOADPREFAB;

    m_Imgui.ImGuiHelp("Load Prefab(s)");

    ImGui::EndMenuBar();
}

void ArchetypeInspector::PrefabPanel()
{
    std::array<const paperback::component::info*, 1 > ComponentAddRemove;

    int Index = 0;
    std::string ArchetypeName;

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
            if (Filter.PassFilter(ArchetypeName.c_str()))
            {
                if (Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
                {
                    auto Prefab = Archetype->FindComponent<prefab>(paperback::vm::PoolDetails{ 0, i });
                    auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });
                    auto Name = Archetype->FindComponent<name>(paperback::vm::PoolDetails({ 0, i }));

                    if (Name)
                    {
                        auto& String = Name->m_Value;
                        char Buffer[256]{};

                        std::copy(String.begin(), String.end(), Buffer);
                        ImGui::Selectable(Name->m_Value.c_str());
                    }
                    else
                    {
                        //Add in missing name component
                        ComponentAddRemove[0] = &paperback::component::info_v<name>;
                        PPB.AddOrRemoveComponents(Entity, ComponentAddRemove, {});
                        
                        if (!m_Imgui.m_Components.empty())
                            m_Imgui.UpdateComponents(Entity.m_GlobalIndex);
                    }

                    DisplayPrefabComponents(Archetype, i, Prefab);

                    if (ImGui::IsItemClicked())
                    {
                        m_Imgui.m_SelectedEntity.first = Archetype;
                        m_Imgui.m_SelectedEntity.second = i;

                        m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
                        //EDITOR_TRACE_PRINT("Editing Prefab: " + Name->m_Value);
                    }

                    if (ImGui::BeginPopupContextItem())
                    {
                        if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE " Clone Prefab"))
                        {
                            auto& EntityName = m_Imgui.m_pArchetype->GetComponent<name>(paperback::vm::PoolDetails{ 0, i });

                            auto& EntityInfo = PPB.GetEntityInfo(Entity.m_GlobalIndex);

                            m_Imgui.m_pArchetype->ClonePrefab(EntityInfo.m_PoolDetails.m_PoolIndex);

                            EDITOR_INFO_PRINT("Prefab Instance:" + EntityName.m_Value + " is added to the scene");
                        }
                        m_Imgui.ImGuiHelp("Spawns an instance of this prefab");


                        if (ImGui::MenuItem(" Remove Prefab"))
                        {
                            std::string Temp = m_Imgui.m_pArchetype->GetComponent<name>(paperback::vm::PoolDetails{ 0, i }).m_Value;

                            ComponentAddRemove[0] = &paperback::component::info_v<prefab>;
                            PPB.AddOrRemoveComponents(Archetype->GetComponent
                                <paperback::component::entity>(paperback::vm::PoolDetails{ 0, i }), {} , ComponentAddRemove);

                            if (!m_Imgui.m_Components.empty())
                                m_Imgui.UpdateComponents(Entity.m_GlobalIndex);

                            EDITOR_WARN_PRINT("Removed Prefab Component from: " + Temp);
                        }
                        m_Imgui.ImGuiHelp("Removes prefab component\n Instances will become normal entities");

                        ImGui::EndPopup();
                    }

                    auto ParentPrefab = Archetype->FindComponent<parent>(paperback::vm::PoolDetails{ 0, i });

                    if (ParentPrefab)
                        m_Imgui.DisplayChildEntities(*ParentPrefab);
                }
            }
        }
    }
}

void ArchetypeInspector::DisplayPrefabComponents(paperback::archetype::instance* Archetype, paperback::u32 i, prefab* Prefab)
{
    m_Imgui.m_pArchetype = Archetype;
    m_Imgui.m_ComponentNames.clear();

    if (ImGui::IsItemHovered())
    {
        for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
            m_Imgui.m_ComponentNames.push_back(Archetype->GetComponentInfos()[i]->m_pName);

        ImGui::BeginTooltip();

        if (!m_Imgui.m_ComponentNames.empty() && m_Imgui.m_pArchetype)
        {
            if (Prefab)
            {
                if (Prefab->m_ReferencePrefabGIDs.size())
                    ImGui::Text("Number of Instances: %d", Prefab->m_ReferencePrefabGIDs.size());
            }
            else
                ImGui::Text("Number of Entities: %d", m_Imgui.m_pArchetype->GetCurrentEntityCount());

            ImGui::Separator();

            ImGui::Text("Archetype Components: ");

            for (auto& Names : m_Imgui.m_ComponentNames)
                ImGui::Text(Names);
        }
        ImGui::EndTooltip();
    }
}