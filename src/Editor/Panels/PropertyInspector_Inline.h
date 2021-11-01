#pragma once
#include "Editor/Panels/PropertyInspector.h"

void DetailsWindow::Panel()
{
	ImGui::Begin(DetailsWindow::typedef_v.m_pName, &m_bEnabled);

	static ImGuiTextFilter Filter;
	Filter.Draw(ICON_FA_FILTER, 150.0f);

	if ( !m_Imgui.m_Components.empty() )
	{
        ImGui::PushItemWidth(120.0f);

		AddComponent(); ImGui::SameLine(); RemoveComponent();

        ImGui::PopItemWidth();

		ImGui::Separator();

		for ( auto& ComponentInstance : m_Imgui.m_Components )
		{
            if ( Filter.PassFilter(ComponentInstance.get_type().get_name().to_string().c_str()) )
            {
                if ( ImGui::CollapsingHeader(ComponentInstance.get_type().get_name().to_string().c_str(), ImGuiTreeNodeFlags_DefaultOpen) )
                {
                    for ( auto& property : ComponentInstance.get_type().get_properties() )
                    {
                        rttr::variant PropertyValue = property.get_value(ComponentInstance);

                        if (!PropertyValue) continue;

                        auto PropertyType = property.get_type(); //etc vector 3, std::string etc
                        auto PropertyName = property.get_name().to_string();

                        if ( PropertyType.get_wrapped_type().is_arithmetic() )
                            m_Imgui.DisplayBaseTypes( PropertyName, PropertyType, PropertyValue );
                        else if ( PropertyType.get_wrapped_type() == rttr::type::get< std::string >() || PropertyType == rttr::type::get< std::string>() )
                            m_Imgui.DisplayStringType( PropertyName, PropertyType, PropertyValue );
                        else if ( PropertyType.is_class() )
                            m_Imgui.DisplayClassType( PropertyName, PropertyType, PropertyValue );

                        if (ComponentInstance.get_type().get_name().to_string() == "Parent")
                            ParentComponent();
                    }
                }
            }
		}
	}

	ImGui::End();
}

void DetailsWindow::AddComponent()
{
    std::array<const paperback::component::info*, 1 > ComponentAdd;

    if (m_Imgui.m_SelectedEntity.first)
    {
        if (ImGui::BeginCombo("##AddComponents", ICON_FA_PLUS " Add"))
        {
            for (auto& [CompKey, CompPInfo] : PPB.GetComponentInfoMap())
            {
                if (!m_Imgui.m_SelectedEntity.first->CheckComponentExistence(CompPInfo))
                {
                    if (ImGui::Selectable(CompPInfo->m_pName))
                    {
                        ComponentAdd[0] = CompPInfo;
                        PPB.AddOrRemoveComponents(m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }), ComponentAdd, {});

                        EDITOR_INFO_PRINT("Added: " + std::string(CompPInfo->m_pName) + " Component");

                        m_Imgui.m_SelectedEntity.first = nullptr;
                        m_Imgui.m_SelectedEntity.second = paperback::u32_max;

                        if (!m_Imgui.m_Components.empty())
                        {
                            m_Imgui.m_Components.clear();
                            ImGui::EndCombo();
                            return;
                        }
                        else
                        {
                            ImGui::EndCombo();
                            return;
                        }
                    }
                }
            }

            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::RemoveComponent()
{
    std::array<const paperback::component::info*, 1 > ComponentRemove;

    if (m_Imgui.m_SelectedEntity.first)
    {
        if (ImGui::BeginCombo("##RemoveComponent", ICON_FA_MINUS " Remove"))
        {
            for (paperback::u32 i = 0; i < m_Imgui.m_SelectedEntity.first->GetComponentCount(); ++i)
            {
                if (ImGui::Selectable(m_Imgui.m_SelectedEntity.first->GetComponentInfos()[i]->m_pName))
                {
                    ComponentRemove[0] = (m_Imgui.m_SelectedEntity.first->GetComponentInfos()[i]);

                    std::string Temp = m_Imgui.m_SelectedEntity.first->GetName();

                    PPB.AddOrRemoveComponents(m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }), {}, ComponentRemove);

                    EDITOR_INFO_PRINT("Removed: " + Temp + " Component");

                    m_Imgui.m_SelectedEntity.first = nullptr;
                    m_Imgui.m_SelectedEntity.second = paperback::u32_max;

                    if (!m_Imgui.m_Components.empty())
                    {
                        m_Imgui.m_Components.clear();
                        ImGui::EndCombo();
                        return;
                    }
                    else
                    {
                        ImGui::EndCombo();
                        return;
                    }
                }
            }

            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::ParentComponent()
{
    bool Remove = false;
    std::array<const paperback::component::info*, 1 > ComponentAdd;

    if (m_Imgui.m_SelectedEntity.first->FindComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second })))
    {
        auto& Parent = m_Imgui.m_SelectedEntity.first->GetComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

        if (Parent.m_ChildrenGlobalIndexes.size() != 0)
        {
            ImGui::Text("There are %d Children attached", Parent.m_ChildrenGlobalIndexes.size());

            for (auto& Child : Parent.m_ChildrenGlobalIndexes)
            {
                ImGui::Text("%d", Child);

                //if (ImGui::BeginPopupContextItem())
                //{
                //    if (ImGui::MenuItem(ICON_FA_TRASH "UnParent?"))
                //    {
                //        Remove = true;
                //    }

                //    ImGui::EndPopup();
                //}

                //if (Remove)
                //    Parent.RemoveChild(Child);
            }
        }
        else
        {
            ImGui::Text("No Child is attached to this parent");

            if (ImGui::BeginCombo("##Potential Children", "Potential Children")) //TBD IDK WHAT TO NAME TIS
            {
                if (!PPB.GetArchetypeList().empty())
                {
                    for (auto& Archetype : PPB.GetArchetypeList())
                    {
                        for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
                        {
                            auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));

                            if (Parent.m_ChildrenGlobalIndexes.find(Entity.m_GlobalIndex) == Parent.m_ChildrenGlobalIndexes.end()) //not already a child of the selected entity
                            {
                                std::string EntityName = Archetype->GetName() + " [" + std::to_string(i) + "]";

                                if (EntityName != (m_Imgui.m_SelectedEntity.first->GetName() + " [" + std::to_string(m_Imgui.m_SelectedEntity.second) + "]"))
                                {
                                    if (ImGui::Selectable(EntityName.c_str()))
                                    {
                                        //auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

                                        if (Archetype->FindComponent<child>(paperback::vm::PoolDetails({ 0, i })) != nullptr) //Check for Child Component
                                        {
                                            //Get Child's Entity Component
                                            auto& ChildEntityComp = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));
                                            //Link Child to Existing Parent
                                            Parent.AddChild(ChildEntityComp.m_GlobalIndex);
                                        }
                                        else
                                        {
                                            // doesnt have a child component
                                            auto& ChildEntityComp = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));
                                            Parent.AddChild(ChildEntityComp.m_GlobalIndex);
                                            //Add in child component
                                            ComponentAdd[0] = &paperback::component::info_v<child>;
                                            PPB.AddOrRemoveComponents(Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i }), ComponentAdd, {});
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                ImGui::EndCombo();
            }

        }
    }
}