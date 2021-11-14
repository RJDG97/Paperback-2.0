#pragma once
#include "Editor/Panels/ArchetypeInspector.h"
#include "Systems/ImguiSystem.h"

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

    if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE))
    {
        PPB.CreatePrefab();
        EDITOR_INFO_PRINT("Added a Default Prefab");
    }

    m_Imgui.ImGuiHelp("Spawns a Default Prefab");


    //if (ImGui::MenuItem(ICON_FA_SAVE))
    //{
    //    m_Imgui.m_Type = FileActivity::SAVEPREFAB;
    //}

    //if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN))
    //    m_Imgui.m_Type = FileActivity::LOADPREFAB;

    //m_Imgui.ImGuiHelp("Saves Prefabs/Archetypes Only");

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

                    auto ParentPrefab = Archetype->FindComponent<parent>(paperback::vm::PoolDetails{ 0, i });
                    if (ParentPrefab)
                    {
                        DisplayPrefabChildren(*ParentPrefab);
                        //Display the kids below
                    }
                }
            }
        }
    }
    ImGui::EndChild();
}

void ArchetypeInspector::ArchetypePanel()
{
    ImGui::BeginChild("Files", { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y}, true);
    ImGui::Text("Prefab Instances");
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

void ArchetypeInspector::DisplayPrefabChildren(parent& Parent)
{
    int Index = 0;
    paperback::u32 ChildToUnlink;

    std::array<const paperback::component::info*, 1 > ComponentRemove;

    paperback::archetype::instance* SelectedChild = nullptr;
    paperback::u32 ChildIndex = paperback::u32_max;

    bool b_NodeOpen = false, Unlink = false;

    if (Parent.m_ChildrenGlobalIndexes.size() != 0)
    {
        for (auto& Child : Parent.m_ChildrenGlobalIndexes)
        {
            Index++;

            auto& ChildEntityInfo = PPB.GetEntityInfo(Child); //get the entity info

            ImGuiTreeNodeFlags NodeFlags = ((m_Imgui.m_SelectedEntity.first == ChildEntityInfo.m_pArchetype && m_Imgui.m_SelectedEntity.second == ChildEntityInfo.m_PoolDetails.m_PoolIndex) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

            auto ChildParent = ChildEntityInfo.m_pArchetype->FindComponent<parent>(ChildEntityInfo.m_PoolDetails);
            if (ChildParent)
                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
            else
                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;


            b_NodeOpen = ImGui::TreeNodeEx((char*)("##" + ChildEntityInfo.m_pArchetype->GetName() + std::to_string(ChildEntityInfo.m_PoolDetails.m_PoolIndex) + std::to_string(Index)).c_str(),
                NodeFlags, (ChildEntityInfo.m_pArchetype->GetName() + " [" + std::to_string(ChildEntityInfo.m_PoolDetails.m_PoolIndex) + "]").c_str());

            if (ImGui::IsItemClicked())
            {
                m_Imgui.m_SelectedEntity.first = ChildEntityInfo.m_pArchetype;
                m_Imgui.m_SelectedEntity.second = ChildEntityInfo.m_PoolDetails.m_PoolIndex;
                m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
            }

            if (ImGui::IsItemHovered()) //view components
            {
                m_Imgui.m_pArchetype = ChildEntityInfo.m_pArchetype;
                m_Imgui.m_ComponentNames.clear();

                for (paperback::u32 i = 0; i < ChildEntityInfo.m_pArchetype->GetComponentCount(); ++i)
                    m_Imgui.m_ComponentNames.push_back(ChildEntityInfo.m_pArchetype->GetComponentInfos()[i]->m_pName);

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

            //bool Deleted = false;

            if (ImGui::BeginPopupContextItem())
            {
                //if (ImGui::MenuItem(ICON_FA_TRASH "Delete?"))
                //{
                //    m_Imgui.m_SelectedEntity.first = ChildEntityInfo.m_pArchetype;
                //    m_Imgui.m_SelectedEntity.second = ChildEntityInfo.m_PoolDetails.m_PoolIndex;
                //    Deleted = true;
                //}

                if (ImGui::MenuItem(ICON_FA_TRASH "UnParent?"))
                {
                    ChildToUnlink = Child;

                    SelectedChild = ChildEntityInfo.m_pArchetype;
                    ChildIndex = ChildEntityInfo.m_PoolDetails.m_PoolIndex;

                    Unlink = true;
                }

                ImGui::EndPopup();
            }

            if (b_NodeOpen)
            {
                if (ChildParent)
                    DisplayPrefabChildren(*ChildParent);

                ImGui::TreePop();
            }
        }
        if (Unlink && SelectedChild)
        {
            Parent.RemoveChild(ChildToUnlink);
            ComponentRemove[0] = &paperback::component::info_v<child>;

            PPB.AddOrRemoveComponents(SelectedChild->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }), {}, ComponentRemove);

            SelectedChild = nullptr;
            ChildIndex = {};

            if (m_Imgui.m_SelectedEntity.first)
            {
                m_Imgui.m_SelectedEntity.first = nullptr;
                m_Imgui.m_SelectedEntity.second = paperback::u32_max;
            }

            if (!m_Imgui.m_Components.empty())
                m_Imgui.m_Components.clear();

            Unlink = false;
        }
    }
}