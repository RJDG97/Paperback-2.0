#include "EntityInspector.h"

void EntityInspector::Panel()
{
    ImGui::Begin(EntityInspector::typedef_v.m_pName, &m_bEnabled);

    DisplayEntities();

    ImGui::End();
}

void EntityInspector::DisplayEntities()
{
    int Index = 0, NumEntities = 0;
    bool b_NodeOpen = false;
    std::array<const paperback::component::info*, 1 > ComponentAdd;

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 150.0f);

    if (!PPB.GetArchetypeList().empty())
    {
        for (auto& Archetype : PPB.GetArchetypeList())
        {
            Index++;

            if (Filter.PassFilter(Archetype->GetName().c_str()))
            {
                for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
                {
                    if ( Archetype->FindComponent<prefab>(paperback::vm::PoolDetails({ 0, i })) == nullptr ) //not a child entity
                    {
                        ++NumEntities;

                        if (Archetype->FindComponent<child>(paperback::vm::PoolDetails({ 0, i })) == nullptr) //not a prefab entity
                        {
                            ImGuiTreeNodeFlags NodeFlags = ((m_Imgui.m_SelectedEntity.first == Archetype && m_Imgui.m_SelectedEntity.second == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow; //update this

                            auto Parent = Archetype->FindComponent<parent>(paperback::vm::PoolDetails({ 0, i }));

                            if (Parent)
                                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                            else
                                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

                            b_NodeOpen = ImGui::TreeNodeEx((char*)("##" + Archetype->GetName() + " [" + std::to_string(i) + std::to_string(Index) + "]").c_str(), NodeFlags, (Archetype->GetName() + " [" + std::to_string(i) + "]").c_str());

                            if (ImGui::IsItemClicked())
                            {
                                m_Imgui.m_SelectedEntity.first = Archetype;
                                m_Imgui.m_SelectedEntity.second = i;
                                m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
                            }

                            bool Deleted = false;

                            if (ImGui::BeginPopupContextItem())
                            {
                                if (!Archetype->GetComponentBits().Has(paperback::component::info_v<reference_prefab>.m_UID))
                                {
                                    if (ImGui::MenuItem("Create as Prefab"))
                                    {
                                        m_Imgui.m_SelectedEntity.first = Archetype;
                                        m_Imgui.m_SelectedEntity.second = i;

                                        ComponentAdd[0] = &paperback::component::info_v<prefab>;

                                        if (!m_Imgui.m_SelectedEntity.first->GetComponentBits().Has(paperback::component::info_v<parent>.m_UID))
                                        {
                                            
                                            PPB.AddOrRemoveComponents(m_Imgui.m_SelectedEntity.first->GetComponent
                                            <paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }), ComponentAdd, {});
                                        }
                                        else
                                        {
                                            //entity is a parent + its a normal entity
                                            auto& Parent = m_Imgui.m_SelectedEntity.first->GetComponent<parent>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

                                            if (Parent.m_ChildrenGlobalIndexes.size() != 0)
                                            {
                                                for (auto& Child : Parent.m_ChildrenGlobalIndexes)
                                                {
                                                    auto& EntityInfo = PPB.GetEntityInfo(Child);

                                                    //add prefab component to the children as well

                                                    PPB.AddOrRemoveComponents(EntityInfo.m_pArchetype->GetComponent<paperback::component::entity>
                                                                             (EntityInfo.m_PoolDetails), ComponentAdd, {});
                                                }
                                            }
                                        }
                                    }
                                }

                                if (ImGui::MenuItem("Clone Entity"))
                                {
                                    m_Imgui.m_SelectedEntity.first = Archetype;
                                    m_Imgui.m_SelectedEntity.second = i;

                                    m_Imgui.m_SelectedEntity.first->CloneEntity(m_Imgui.m_SelectedEntity.first->GetComponent
                                        <paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }));
                                }

                                ImGui::Separator();

                                if (ImGui::MenuItem(ICON_FA_TRASH "Delete?"))
                                {
                                    m_Imgui.m_SelectedEntity.first = Archetype;
                                    m_Imgui.m_SelectedEntity.second = i;
                                    Deleted = true;
                                }

                                ImGui::EndPopup();
                            }

                            if (b_NodeOpen)
                            {
                                if (Parent)
                                    m_Imgui.DisplayChildEntities(*Parent);

                                ImGui::TreePop();
                            }

                            if (Deleted)
                                ImGui::OpenPopup(ICON_FA_TRASH " Delete?");
                        }
                    }
                }
            }
        }

        DeleteEntity(ICON_FA_TRASH " Delete?", m_Imgui.m_SelectedEntity.second);
    }

    //ImGui::Separator();
    //ImGui::Text("%d Entities", NumEntities);
}


void EntityInspector::DeleteEntity(std::string WindowName, paperback::u32 EntityIndex)
{
    if (m_Imgui.m_SelectedEntity.first)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Deleting: ");

            ImGui::Text((m_Imgui.m_SelectedEntity.first->GetName() + " [" + std::to_string(m_Imgui.m_SelectedEntity.second) + "]").c_str());

            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "This cannot be undone");

            ImGui::Separator();

            if (ImGui::Button("Yes"))
            {
                auto& Entity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, EntityIndex });
                auto RefPrefab = m_Imgui.m_SelectedEntity.first->FindComponent<reference_prefab>(paperback::vm::PoolDetails{ 0, EntityIndex });

                if (RefPrefab)
                {
                    //Get referenced prefab gid + EntityInfo

                    auto& Info = PPB.GetEntityInfo(RefPrefab->m_PrefabGID);
                    //unlink prefab reference link from the main prefab
                    Info.m_pArchetype->GetComponent<prefab>(Info.m_PoolDetails).RemovePrefabInstance(Entity.m_GlobalIndex);
                }

                PPB.DeleteEntity(Entity);
                EDITOR_INFO_PRINT("Deleted: " + m_Imgui.m_SelectedEntity.first->GetName() + " [" + std::to_string(m_Imgui.m_SelectedEntity.second) + "]");

                m_Imgui.m_SelectedEntity = { nullptr, paperback::u32_max };

                if (!m_Imgui.m_Components.empty())
                    m_Imgui.m_Components.clear();


                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(0, 4);

            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
    }
}
