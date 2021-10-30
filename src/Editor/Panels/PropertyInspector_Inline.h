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
                        else if ( PropertyType.get_wrapped_type() == rttr::type::get< std::string >() )
                            m_Imgui.DisplayStringType( PropertyName, PropertyType, PropertyValue );
                        else if ( PropertyType.is_class() )
                            m_Imgui.DisplayClassType( PropertyName, PropertyType, PropertyValue );
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
            for (auto& [CompKey, CompValue] : PPB.GetComponentInfoMap())
            {
                if (!m_Imgui.m_SelectedEntity.first->CheckComponentExistence(CompValue))
                {
                    if (ImGui::Selectable(CompValue->m_pName))
                    {
                        ComponentAdd[0] = CompValue;
                        PPB.AddOrRemoveComponents(m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }), ComponentAdd, {});

                        EDITOR_INFO_PRINT("Added: " + std::string(CompValue->m_pName) + " Component");

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