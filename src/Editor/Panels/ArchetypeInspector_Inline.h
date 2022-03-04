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
        EDITOR_INFO_PRINT("Blank Prefab created");
    }

    m_Imgui.ImGuiHelp("Creates a Blank Prefab");

    if (ImGui::MenuItem(ICON_FA_SAVE))
        m_Imgui.m_Type = FileActivity::SAVEPREFAB;

    m_Imgui.ImGuiHelp("Saves ALL Prefabs in the Inspector");

    if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN))
        m_Imgui.m_Type = FileActivity::LOADPREFAB;

    m_Imgui.ImGuiHelp("Load Prefab(s)");

    ImGui::EndMenuBar();
}

void ArchetypeInspector::PrefabPanel()
{
    std::array<const paperback::component::info*, 1 > ComponentAddRemove;
    bool bOpen = false;
    int Index = 0;
    std::string ArchetypeName;

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 170.0f);

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Add Blank Prefab"))
        {
            PPB.CreatePrefab();
            EDITOR_INFO_PRINT("Blank Prefab created");
        }

        m_Imgui.ImGuiHelp("Creates a Blank Prefab");

        ImGui::EndPopup();
    }


    for (auto& Archetype : PPB.GetArchetypeList())
    {
        ++Index;

        for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
        {
            if (Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID)) //has a prefab component
            {
                if (Archetype->FindComponent<child>(paperback::vm::PoolDetails{ 0, i }) == nullptr) // isnt a child -> child prefab will be displayed through their parents
                {
                    auto Prefab = Archetype->FindComponent<prefab>(paperback::vm::PoolDetails{ 0, i });
                    auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });
                    auto Name = Archetype->FindComponent<name>(paperback::vm::PoolDetails({ 0, i }));
                    auto ParentPrefab = Archetype->FindComponent<parent>(paperback::vm::PoolDetails{ 0, i });

                    if (Name)
                        ArchetypeName = Name->m_Value;
                    else
                    {
                        //Add in missing name component
                        ComponentAddRemove[0] = &paperback::component::info_v<name>;
                        PPB.AddOrRemoveComponents(Entity, ComponentAddRemove, {});

                        if (!m_Imgui.m_Components.empty())
                            m_Imgui.UpdateComponents(Entity.m_GlobalIndex);
                    }

                    if (Filter.PassFilter(ArchetypeName.c_str()))
                    {
                        ImGuiTreeNodeFlags NodeFlags = ((m_Imgui.m_SelectedEntity.first == Archetype && m_Imgui.m_SelectedEntity.second == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

                        if (ParentPrefab)
                            NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                        else
                            NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

                        bOpen = ImGui::TreeNodeEx((char*)("##" + Archetype->GetName() + " [" + std::to_string(i) + std::to_string(Index) + "]").c_str(), NodeFlags, ArchetypeName.c_str());

                        DisplayPrefabComponents(Archetype, i, Prefab);

                        if (ImGui::IsItemClicked())
                        {
                            m_Imgui.m_SelectedEntity.first = Archetype;
                            m_Imgui.m_SelectedEntity.second = i;

                            m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
                            EDITOR_TRACE_PRINT("Editing Prefab: " + ArchetypeName);
                        }

                        if (ImGui::BeginPopupContextItem())
                        {
                            //m_Imgui.m_pArchetype = Archetype;

                            if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE " Clone Prefab"))
                            {
                                auto& EntityInfo = PPB.GetEntityInfo(Entity.m_GlobalIndex);

                                m_Imgui.m_pArchetype->ClonePrefab(EntityInfo.m_PoolDetails.m_PoolIndex);

                                EDITOR_INFO_PRINT("Prefab Instance:" + ArchetypeName + " is added to the scene");
                            }
                            m_Imgui.ImGuiHelp("Spawns an instance of this prefab");


                            if (!ParentPrefab || !ParentPrefab->m_ChildrenGlobalIndexes.size())
                            {
                                if (ImGui::MenuItem("Remove Prefab"))
                                {
                                    std::string Temp = m_Imgui.m_pArchetype->GetComponent<name>(paperback::vm::PoolDetails{ 0, i }).m_Value;

                                    //Abandon all instances before removing the prefab component
                                    if (Prefab->m_ReferencePrefabGIDs.size())
                                    {
                                        for (auto& Instance : Prefab->m_ReferencePrefabGIDs)
                                        {
                                            ComponentAddRemove[0] = &paperback::component::info_v<reference_prefab>;
                                            auto& InstanceEntityInfo = PPB.GetEntityInfo(Instance);

                                            PPB.AddOrRemoveComponents(InstanceEntityInfo.m_pArchetype->GetComponent<paperback::component::entity>(InstanceEntityInfo.m_PoolDetails),
                                                {}, ComponentAddRemove);
                                        }

                                        //clear the list
                                        Prefab->m_ReferencePrefabGIDs.clear();
                                    }

                                    //Remove prefab component
                                    ComponentAddRemove[0] = &paperback::component::info_v<prefab>;
                                    PPB.AddOrRemoveComponents(Archetype->GetComponent
                                        <paperback::component::entity>(paperback::vm::PoolDetails{ 0, i }), {}, ComponentAddRemove);

                                    if (!m_Imgui.m_Components.empty())
                                        m_Imgui.m_Components.clear();

                                    m_Imgui.m_pArchetype = nullptr;

                                    EDITOR_WARN_PRINT("Removed Prefab Component from: " + Temp);
                                }


                                ImGui::Separator();

                                if (ImGui::MenuItem(ICON_FA_SAVE " Save this prefab"))
                                {
                                    m_Imgui.m_SelectedEntity.first = Archetype;
                                    m_Imgui.m_SelectedEntity.second = i;

                                    m_Imgui.m_Type = FileActivity::SAVEINDIVIDUALPREFAB;
                                }

                                m_Imgui.ImGuiHelp("Saves the selected prefab");
                            }

                            ImGui::EndPopup();
                        }

                        if (bOpen)
                        {
                            if (ParentPrefab)
                                m_Imgui.DisplayChildEntities(*ParentPrefab);
                            ImGui::TreePop();
                        }
                    }
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