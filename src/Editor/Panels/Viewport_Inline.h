#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName, &m_bEnabled);

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();

	ImGui::End();
}

void EditorViewport::Gizmo()
{
	glm::mat4 t{ 1.0f };

	if (m_Imgui.m_SelectedEntity.first) //An entity is selected	
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight); //setting the viewport

		//Camera
		glm::mat4 CameraView = Camera::GetInstanced().GetView();
		glm::mat4 CameraProjection = Camera::GetInstanced().GetProjection();

		//Entity
		//Get Transform component
		rttr::instance EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

		for (auto& Property : EntityTransform.get_type().get_properties())
		{
			rttr::variant PropertyValue = Property.get_value(EntityTransform);

			if (Property.get_name() == "Position")
			{
				auto& Position = PropertyValue.get_value<std::reference_wrapper<paperback::Vector3f>>().get();
				t = glm::translate(t, glm::vec3(Position.x, Position.y, Position.z));
			}
		}

		ImGuizmo::Manipulate( glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL,
			glm::value_ptr(t));
		
	}
}