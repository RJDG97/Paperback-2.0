#include "EntityInspector.h"

void EntityInspector::Panel()
{
    ImGui::Begin(EntityInspector::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

    //if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    //{
    //    m_Imgui.m_SelectedEntity.first = {};

    //    if (!m_Imgui.m_Components.empty())
    //        m_Imgui.m_Components.clear();
    //}

    DisplayEntities();

    ImGui::End();
}

void EntityInspector::DisplayEntities()
{
    int Index = 0, NumEntities = 0;
    std::array<const paperback::component::info*, 1 > ComponentAddRemove;
    //paperback::u32 DragDropEntityGID;
    /*bool Link = false*/bool bOpen = false;
    std::string EntityName;

    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 150.0f);

    if (!PPB.GetArchetypeList().empty())
    {
        for (auto& Archetype : PPB.GetArchetypeList())
        {
            for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
            {
                Index++;

                if ( Archetype->FindComponent<prefab>(paperback::vm::PoolDetails({ 0, i })) == nullptr ) //not a child entity
                {
                    if (Archetype->FindComponent<child>(paperback::vm::PoolDetails({ 0, i })) == nullptr) //not a prefab entity
                    {
                        auto Name = Archetype->FindComponent<name>(paperback::vm::PoolDetails({ 0, i }));
                        auto Parent = Archetype->FindComponent<parent>(paperback::vm::PoolDetails({ 0, i }));
                        auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });

                        if (Name)
                            EntityName = Name->m_Value;
                        else
                        {
                            //Add in missing name component
                            ComponentAddRemove[0] = &paperback::component::info_v<name>;
                            PPB.AddOrRemoveComponents(Entity, ComponentAddRemove, {});

                            if (!m_Imgui.m_Components.empty())
                                m_Imgui.UpdateComponents(Entity.m_GlobalIndex);
                        }

                        if (Filter.PassFilter(EntityName.c_str()))
                        {
                            ImGuiTreeNodeFlags NodeFlags = ((m_Imgui.m_SelectedEntity.first == Archetype && m_Imgui.m_SelectedEntity.second == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

                            if (Parent)
                                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                            else
                                NodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

                            bOpen = ImGui::TreeNodeEx((char*)("##" + Archetype->GetName() + " [" + std::to_string(i) + std::to_string(Index) + "]").c_str(), NodeFlags, EntityName.c_str());

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

                                        ComponentAddRemove[0] = &paperback::component::info_v<prefab>;
                                        auto Parent = m_Imgui.m_SelectedEntity.first->FindComponent<parent>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
                                        std::string Temp = EntityName;

                                        if (!Parent)
                                        {
                                            //Not a parent + normal entity
                                            PPB.AddOrRemoveComponents(Entity, ComponentAddRemove, {});

                                            if (!m_Imgui.m_Components.empty())
                                                m_Imgui.UpdateComponents(Entity.m_GlobalIndex);
                                        }
                                        else
                                        {
                                            //entity is a parent + its a normal entity
                                            if (Parent->m_ChildrenGlobalIndexes.size())
                                            {
                                                for (auto& Child : Parent->m_ChildrenGlobalIndexes)
                                                {
                                                    auto& EntityInfo = PPB.GetEntityInfo(Child);

                                                    //add prefab component to the children as well
                                                    PPB.AddOrRemoveComponents(EntityInfo.m_pArchetype->GetComponent<paperback::component::entity>
                                                                             (EntityInfo.m_PoolDetails), ComponentAddRemove, {});
                                                }
                                            }
                                        }

                                        EDITOR_INFO_PRINT("Added as a Prefab: " + Temp);
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

                            //if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_SourceNoDisableHover))
                            //{
                            //    DragDropEntityGID = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i }).m_GlobalIndex;

                            //    ImGui::SetDragDropPayload("Potential Child", &DragDropEntityGID, sizeof(paperback::u32));
                            //    if (Name)
                            //        ImGui::Text(Name->m_Value.c_str());
                            //    ImGui::EndDragDropSource();
                            //}

                            //if (ImGui::BeginDragDropTarget())
                            //{
                            //    //Entity being dropped onto
                            //    auto& SelectedEntity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });
                            //    
                            //    if (Parent)
                            //    {
                            //        //if have parent component
                            //        if (DragDropEntityGID == SelectedEntity.m_GlobalIndex) //If dropping onto itself
                            //            Link = false;
                            //    }
                            //    else
                            //    {
                            //        //no parent component -> Can just link
                            //        std::array<const paperback::component::info*, 1 > ComponentAdd = &paperback::component::info_v<parent>;
                            //    }
                            //    ImGui::EndDragDropTarget();
                            //}

                            if (bOpen)
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
    std::string Temp = {};

    if (m_Imgui.m_SelectedEntity.first)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(WindowName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Deleting: ");

            auto EntityName = m_Imgui.m_SelectedEntity.first->FindComponent<name>(paperback::vm::PoolDetails{ 0, EntityIndex });

            if (EntityName) Temp = EntityName->m_Value;

            ImGui::Text(Temp.c_str());

            ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "This cannot be undone");

            ImGui::Separator();

            if (ImGui::Button("Yes"))
            {
                auto& Entity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, EntityIndex });

                PPB.DeleteEntity(Entity);
                EDITOR_INFO_PRINT("Deleted: " + Temp);

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
