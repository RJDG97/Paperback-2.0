#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include "glm/inc/gtx/matrix_decompose.hpp"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_MenuBar);

	ViewportMenuBar();

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();

	ImGui::End();
}

void EditorViewport::ViewportMenuBar()
{
	ImGui::BeginMenuBar();

	if (ImGui::MenuItem(ICON_FA_ARROWS_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	
	}
	if (ImGui::MenuItem(ICON_FA_EXPAND_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	if (ImGui::MenuItem(ICON_FA_SYNC_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
	}

	if (ImGui::MenuItem(ICON_FA_BROOM))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = -1;
	}

	ImGui::EndMenuBar();
}

void EditorViewport::Gizmo()
{
	if (m_Imgui.m_SelectedEntity.first && m_GizmoType != -1) //An entity is selected	
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight); //setting the viewport

		ComposeTransform();
	}
}

void EditorViewport::ComposeTransform()
{
	bool bTrans = false, bScale = false, bRot = false;
	glm::mat4 model{ 1.0f };
	glm::vec3 Trans, Rot, Scale;

	//Camera
	glm::mat4 CameraView = Camera::GetInstanced().GetView();
	glm::mat4 CameraProjection = Camera::GetInstanced().GetProjection();

	auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });

	if (m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails) != nullptr)
		bTrans = true;
	if (m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails) != nullptr)
		bRot = true;
	if (m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails) != nullptr)
		bScale = true;

	if (bTrans && bRot && bScale)
	{
		transform& EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
		model = glm::translate(model, glm::vec3(EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z));

		rotation& EntityRotate = m_Imgui.m_SelectedEntity.first->GetComponent<rotation>(EntityDetails);
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

		scale& EntityScale = m_Imgui.m_SelectedEntity.first->GetComponent<scale>(EntityDetails);
		model = glm::scale(model, glm::vec3(EntityScale.m_Value.x, EntityScale.m_Value.y, EntityScale.m_Value.z));

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);

			glm::vec3 TempRot = Rot - glm::vec3(glm::radians(EntityRotate.m_Value.x), glm::radians(EntityRotate.m_Value.y), glm::radians(EntityRotate.m_Value.z));

			Editor::Math::GlmtoVec3(EntityTransform.m_Position, Trans);

			EntityRotate.m_Value.x += TempRot.x;
			EntityRotate.m_Value.y += TempRot.y;
			EntityRotate.m_Value.z += TempRot.z;

			Editor::Math::GlmtoVec3(EntityScale.m_Value, Scale);
		}
	}

	if (bTrans && !bRot && !bScale)
	{
		transform& EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
		model = glm::translate(model, glm::vec3(EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z));

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

		model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);
			Editor::Math::GlmtoVec3(EntityTransform.m_Position, Trans);
		}
	}

	if (bTrans && !bRot && bScale)
	{
		transform& EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
		model = glm::translate(model, glm::vec3(EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z));

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

		scale& EntityScale = m_Imgui.m_SelectedEntity.first->GetComponent<scale>(EntityDetails);
		model = glm::scale(model, glm::vec3(EntityScale.m_Value.x, EntityScale.m_Value.y, EntityScale.m_Value.z));

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);

			Editor::Math::GlmtoVec3(EntityTransform.m_Position, Trans);

			Editor::Math::GlmtoVec3(EntityScale.m_Value, Scale);
		}
	}

	if (bTrans && bRot && !bScale)
	{
		transform& EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
		model = glm::translate(model, glm::vec3(EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z));

		rotation& EntityRotate = m_Imgui.m_SelectedEntity.first->GetComponent<rotation>(EntityDetails);
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(EntityRotate.m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

		model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

		ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);

			glm::vec3 TempRot = Rot - glm::vec3(glm::radians(EntityRotate.m_Value.x), glm::radians(EntityRotate.m_Value.y), glm::radians(EntityRotate.m_Value.z));

			Editor::Math::GlmtoVec3(EntityTransform.m_Position, Trans);

			EntityRotate.m_Value.x += TempRot.x;
			EntityRotate.m_Value.y += TempRot.y;
			EntityRotate.m_Value.z += TempRot.z;
		}
	}



}